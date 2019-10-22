/*
Copyright 2019 bemeier

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// TODO: expand on this with an easy lib to be included in keymaps.
// This may belong to the "/user" space...
#pragma once

#ifndef BMLED_SMOOTH_NVALS
  #define BMLED_SMOOTH_NVALS 1
#endif

#ifndef BMLED_SMOOTH_INTERVAL
  #define BMLED_SMOOTH_INTERVAL 20
#endif

typedef struct {
    uint8_t current_value;
    uint8_t target_value;
    uint8_t max_value;
    uint8_t min_value;
    float smooth_param;
} bmled_smooth;

static uint16_t bmled_smooth_last_update;

uint8_t bmled_get_val(uint8_t val);
void bmled_smooth_init(uint8_t val, uint8_t start_val, uint8_t start_target, uint8_t min, uint8_t max, float param);
void bmled_smooth_add_target(uint8_t val, int add);
void bmled_smooth_set_target(uint8_t val, uint8_t target);
bool bmled_smooth_update(void);

bmled_smooth bmled_smooth_vals[BMLED_SMOOTH_NVALS];

// Initialize a smooth value
void bmled_smooth_init(uint8_t val, uint8_t start_val, uint8_t start_target, uint8_t min, uint8_t max, float param) {
  if (val >= BMLED_SMOOTH_NVALS) return;
  bmled_smooth_last_update = timer_read();
  bmled_smooth_vals[val].current_value = start_val;
  bmled_smooth_vals[val].target_value = start_target;
  bmled_smooth_vals[val].min_value = min;
  bmled_smooth_vals[val].max_value = max;
  bmled_smooth_vals[val].smooth_param = param;
}

// update all smoothed values
bool bmled_smooth_update(void) {
  if (timer_elapsed(bmled_smooth_last_update) > BMLED_SMOOTH_INTERVAL) {
    bmled_smooth_last_update = timer_read();
    for (uint8_t valIdx = 0; valIdx < BMLED_SMOOTH_NVALS; valIdx++) {
      bmled_smooth_vals[valIdx].current_value = bmled_smooth_vals[valIdx].current_value * (1-bmled_smooth_vals[valIdx].smooth_param) +
                                              bmled_smooth_vals[valIdx].target_value * bmled_smooth_vals[valIdx].smooth_param;
    }
    return true;
  }
  return false;
}

void bmled_smooth_add_target(uint8_t val, int add) {
  if (val >= BMLED_SMOOTH_NVALS) return;
  int new_val = bmled_smooth_vals[val].target_value + add;
  if (new_val > bmled_smooth_vals[val].max_value) bmled_smooth_vals[val].target_value = bmled_smooth_vals[val].max_value;
  else if (new_val < bmled_smooth_vals[val].min_value) bmled_smooth_vals[val].target_value = bmled_smooth_vals[val].min_value;
  else bmled_smooth_vals[val].target_value = (uint8_t) new_val;
}

void bmled_smooth_set_target(uint8_t val, uint8_t target) {
  if (val >= BMLED_SMOOTH_NVALS) return;
  if (target > bmled_smooth_vals[val].max_value) bmled_smooth_vals[val].target_value = bmled_smooth_vals[val].max_value;
  else if (target < bmled_smooth_vals[val].min_value) bmled_smooth_vals[val].target_value = bmled_smooth_vals[val].min_value;
  else bmled_smooth_vals[val].target_value = target;
}

uint8_t bmled_get_target(uint8_t val) {
  if (val >= BMLED_SMOOTH_NVALS) return 0;
  return bmled_smooth_vals[val].target_value;
}

uint8_t bmled_get_val(uint8_t val) {
  if (val >= BMLED_SMOOTH_NVALS) return 0;
  return bmled_smooth_vals[val].current_value;
}
