/* Copyright 2019 bemeier
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "bmek.h"
#include "../bmstatus.h"

typedef union {
  uint32_t raw;
  struct {
    uint8_t layer_hues[4];
  };
} kb_config_t;

kb_config_t kb_config;


enum bmled_smooth_vals {
  total_brightness = 0,
  total_sat,
  layer_hue
};

static bool idle = false;
static uint16_t idle_timer;
static uint16_t idle_timeout = 30000;

static bool settings_dirty = false;
static uint8_t cfg_layer_idx = 0;

// TODO: move to eeprom
static uint16_t default_saturation = 245;
static uint16_t default_brightness = 220;
static uint16_t cutoff_brightness = 5;


void eeconfig_init_kb(void) {  // EEPROM is getting reset!
  kb_config.raw = 0;
  kb_config.layer_hues[0] = 100;
  kb_config.layer_hues[1] = 120;
  kb_config.layer_hues[2] = 180;
  kb_config.layer_hues[3] = 210;
  eeconfig_update_kb(kb_config.raw); // Write default value to EEPROM now
}


void matrix_init_kb(void) {
  // idle timer
  kb_config.raw = eeconfig_read_kb();
  idle_timer = timer_read();
  // Setup smoothing engine
  bmled_smooth_init(total_brightness, cutoff_brightness, default_brightness, cutoff_brightness, 255, 0.1);
  bmled_smooth_init(total_sat, 0, default_saturation, 0, 255, 0.2);
  bmled_smooth_init(layer_hue, 0, kb_config.layer_hues[eeconfig_read_default_layer()], 1, 255, 0.25);
  return matrix_init_user();
}


layer_state_t default_layer_state_set_kb(layer_state_t state) {
  return default_layer_state_set_user(state);
}


layer_state_t layer_state_set_kb(layer_state_t state) {
  return layer_state_set_user(state);
}

void matrix_scan_kb(void) {
  layer_state_t hl = get_highest_layer(layer_state);
  layer_state_t hdl = get_highest_layer(default_layer_state);
  if (hl > hdl && hl <= 3) {
    bmled_smooth_set_target(layer_hue, kb_config.layer_hues[hl]);
  } else if (hdl <= 3 && hl < 3) {
    bmled_smooth_set_target(layer_hue, kb_config.layer_hues[hdl]);
    if (settings_dirty) {
      settings_dirty = false;
      eeconfig_update_kb(kb_config.raw);
    }
  } else {
    bmled_set(layer_hue, kb_config.layer_hues[cfg_layer_idx]);
  }

  if (bmled_smooth_update()) {
    rgblight_sethsv(bmled_get_val(layer_hue), bmled_get_val(total_sat), bmled_get_val(total_brightness));

    if (!idle && bmled_get_val(total_brightness) <= cutoff_brightness && bmled_get_target(total_brightness) <= cutoff_brightness) {
        idle = true;
        rgblight_disable_noeeprom();
    }
  }

  if (!idle && timer_elapsed(idle_timer) > idle_timeout) {
    bmled_smooth_set_target(total_brightness, cutoff_brightness);
  }

  return matrix_scan_user();
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
  if (timer_elapsed(idle_timer) >= idle_timeout) {
    if (idle) {
        rgblight_enable_noeeprom();
        bmled_set(total_brightness, cutoff_brightness);
    }
    idle = false;
    idle_timer = timer_read();
    bmled_smooth_set_target(total_brightness, default_brightness);
    bmled_smooth_set_target(total_sat, 255);
  }


  switch (keycode) {
    case BM_WIPE:
      if (record->event.pressed) break;
      eeconfig_init();
      break;
    case BM_RST:
      if (record->event.pressed) break;
      rgblight_sethsv(3, 255, 255);
      reset_keyboard();
  }

  if (keycode >= BM_HI_1 && keycode <= BM_HD_4) {
    settings_dirty = true;
    int delta = 3;
    cfg_layer_idx = keycode - BM_HI_1;
    if (keycode > BM_HI_4) {
      delta = -delta;
      cfg_layer_idx = keycode - BM_HD_1;
    }
    int new_val = kb_config.layer_hues[cfg_layer_idx] + delta;
    if (new_val > 255) new_val = 0;
    if (new_val < 0) new_val = 255;
    kb_config.layer_hues[cfg_layer_idx] = new_val;
  }

  return process_record_user(keycode, record);
}

// Optional override functions below.
// You can leave any or all of these undefined.
// These are only required if you want to perform custom actions.

/*

void matrix_init_kb(void) {
  // put your keyboard start-up code here
  // runs once when the firmware starts up

  matrix_init_user();
}

void matrix_scan_kb(void) {
  // put your looping keyboard code here
  // runs every cycle (a lot)

  matrix_scan_user();
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
  // put your per-action keyboard code here
  // runs for every action, just before processing by the firmware

  return process_record_user(keycode, record);
}

void led_set_kb(uint8_t usb_led) {
  // put your keyboard LED indicator (ex: Caps Lock LED) toggling code here

  led_set_user(usb_led);
}

*/
