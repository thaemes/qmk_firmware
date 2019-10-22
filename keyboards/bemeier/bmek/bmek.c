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

enum bmled_smooth_vals {
  total_brightness = 0,
  type_pulse,
  layer_hue
};

uint32_t idle_timeout = 42000;//1400 * 30;
uint32_t no_keypress_since = 0;
bool idle = false;


void matrix_init_kb(void) {
  // Setup smoothing engine
  bmled_smooth_init(total_brightness, 0, 230, 0, 230, 0.185);
  bmled_smooth_init(type_pulse, 235, 235, 80, 255, 0.1);
  bmled_smooth_init(layer_hue, 0, 0, 1, 255, 0.15);
  //bmled_smooth_init(layer_hue, 0, layer_hues[biton32(eeconfig_read_default_layer())], 1, 255, 0.15);
  return matrix_init_user();
}

void matrix_scan_kb(void) {
  // TODO: maybe comment this out?
  if (idle) return matrix_scan_user();

  if (bmled_smooth_update()) {
    bmled_smooth_add_target(type_pulse, 4); // over time, the type-pulse-light resaturates
    rgblight_sethsv_master(bmled_get_val(layer_hue), 235, bmled_get_val(total_brightness));

    if (!idle && bmled_get_val(total_brightness) <= 1 && bmled_get_target(total_brightness) == 0) {
        idle = true;
        rgblight_disable_noeeprom();
    }
  }

  if (no_keypress_since > idle_timeout) {
    bmled_smooth_set_target(total_brightness, 0);
  } else {
    no_keypress_since++;
  }

  return matrix_scan_user();
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
  if (no_keypress_since >= idle_timeout) {
    if (idle) {
      rgblight_enable_noeeprom();
      idle = false;
    }
    bmled_smooth_set_target(total_brightness, 230);
  }
  no_keypress_since = 0;

  // TODO: can we solve layer specific colors here?
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
