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
#include "../bmledlib.h"

void bm_update_led_indicators(void);
kb_config_t kb_config;

// Smooth LED transitions
enum bmled_smooth_vals {
  smooth_brightness = 0,
  smooth_saturation,
  smooth_layer_hue
};

// Idle Timers
static bool idle = false;
static uint32_t idle_timer;
// TODO: make this configurable from keymap/rules.mk?
static uint32_t idle_timeout = 120000;

// Config
static bool settings_dirty = false;
static uint8_t cfg_layer_idx = 0;

// Macros
static bool recording = false;

void eeconfig_init_kb(void) {
  kb_config.raw_kb = 0;
  kb_config.raw_user = 0;
  kb_config.layer_hues[0] = 100;
  kb_config.layer_hues[1] = 120;
  kb_config.layer_hues[2] = 180;
  kb_config.layer_hues[3] = 210;
  kb_config.brightness = 255;
  kb_config.saturation = 255;
  kb_config.fade_span = 25;
  eeconfig_update_kb(kb_config.raw_kb);
  eeconfig_update_user(kb_config.raw_user);
}

void matrix_init_kb(void) {
  // idle timer (turns of lights after idle_timeout ms)
  kb_config.raw_kb = eeconfig_read_kb();
  kb_config.raw_user = eeconfig_read_user();
  idle_timer = timer_read32();

  // Setup smoothing engine
  bmled_smooth_init(smooth_brightness, 0, kb_config.brightness, 0, 255, 0.2, false);
  bmled_smooth_init(smooth_saturation, 0, kb_config.saturation, 0, 255, 0.3, false);
  bmled_smooth_init(smooth_layer_hue, 0, kb_config.layer_hues[eeconfig_read_default_layer()], 1, 255, 0.25, true);
  rgblight_enable_noeeprom();
  return matrix_init_user();
}

void matrix_scan_kb(void) {
  bm_update_led_indicators();
  return matrix_scan_user();
}

void dynamic_macro_record_start_user(void) {
	recording = true;
}

void dynamic_macro_record_key_user(int8_t direction, keyrecord_t *record) {
}

void dynamic_macro_record_end_user(int8_t dir) {
	recording = false;
}

void dynamic_macro_play_user(int8_t dir) {
}



bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
  // Wake up from idle on keypress, re-enable lights and reset timer
  if (idle || timer_elapsed32(idle_timer) >= idle_timeout) {
    if (idle) rgblight_enable_noeeprom();
    idle = false;
    idle_timer = timer_read32();
  }

  // Config keycodes
  switch (keycode) {
    case BM_RST: // RESET KEYBOARD
      if (record->event.pressed) break;
      rgblight_sethsv(250, 255, 50);
      reset_keyboard();
      break;
    case BM_WIPE: // RESET EEPROM
      if (record->event.pressed) break;
      eeconfig_init();
      break;
	case BM_HI_1 ... BM_HD_4: // SET LAYER COLORS
      settings_dirty = true;
      int delta = 1;
      cfg_layer_idx = keycode - BM_HI_1;
      if (keycode > BM_HI_4) {
        delta = -delta;
        cfg_layer_idx = keycode - BM_HD_1;
      }
      uint8_t new_val = (kb_config.layer_hues[cfg_layer_idx] + delta) % 255;
      kb_config.layer_hues[cfg_layer_idx] = new_val;
      break;
    case BM_SI ... BM_FD: // BRIGHTNESS, SATURATION & INDICATOR FADE
	  settings_dirty = true;
	  int delta_sb = 2;
      if (keycode == BM_BI) bmled_smooth_add_target(smooth_brightness, delta_sb);
      if (keycode == BM_BD) bmled_smooth_add_target(smooth_brightness, -delta_sb);
      if (keycode == BM_SI) bmled_smooth_add_target(smooth_saturation,  delta_sb);
      if (keycode == BM_SD) bmled_smooth_add_target(smooth_saturation, -delta_sb);
      kb_config.brightness = bmled_get_target(smooth_brightness);
      kb_config.saturation = bmled_get_target(smooth_saturation);
      if (keycode == BM_FI) kb_config.fade_span += delta_sb;
      if (keycode == BM_FD) kb_config.fade_span -= delta_sb;
	  kb_config.fade_span = (uint8_t) fmax(fmin(kb_config.fade_span, 169), 0);
	  break;
  }

  return process_record_user(keycode, record);
}

void bm_update_led_indicators(void) {
  // get the highest active (default) layer to set the color indicator...
  layer_state_t hl = get_highest_layer(layer_state);
  layer_state_t hdl = get_highest_layer(default_layer_state);
  if (recording) {
	// Do nothing, target is updated below...
  } else if (hl > hdl && hl <= 3) { // only first 4 layers supported since we cant store more than 4 bytes of hue data in eeprom easily(?)
    bmled_smooth_set_target(smooth_layer_hue, kb_config.layer_hues[hl]);
  } else if (hdl <= 3 && hl < 3) {
    bmled_smooth_set_target(smooth_layer_hue, kb_config.layer_hues[hdl]);
    if (settings_dirty) {
      settings_dirty = false;
      eeconfig_update_kb(kb_config.raw_kb);
      eeconfig_update_user(kb_config.raw_user);
    }
  } else { // If on a higher layer, we assume it's the config layer and instead we show the currently configured hue:
    bmled_set(smooth_layer_hue, kb_config.layer_hues[cfg_layer_idx]);
  }

  if (bmled_smooth_update()) {
    uint8_t span = kb_config.fade_span;
	if (recording) {
	  bmled_smooth_add_target(smooth_layer_hue, 7);
	  span = 64;
    }
	uint8_t current_hue = bmled_get_val(smooth_layer_hue);
    int half_leds = RGBLED_NUM/2;
	for (int i = 0; i < half_leds; i++) {
		uint8_t this_hue = current_hue + span * i/(RGBLED_NUM/2) % 255;
    	rgblight_sethsv_at(this_hue, bmled_get_val(smooth_saturation), bmled_get_val(smooth_brightness), i);
    	rgblight_sethsv_at(this_hue, bmled_get_val(smooth_saturation), bmled_get_val(smooth_brightness), i+half_leds);
	}


    if (!idle && timer_elapsed32(idle_timer) >= idle_timeout) {
		// fade out when the timer is reached:
        bmled_smooth_set_target(smooth_brightness, 0);
		// turn of led's completely after fade is complete:
		if (bmled_get_val(smooth_brightness) <= 1) {
          rgblight_disable_noeeprom();
          idle = true;
        }
    } else if (!idle) {
      bmled_smooth_set_target(smooth_brightness, kb_config.brightness);
    }
  }
}
