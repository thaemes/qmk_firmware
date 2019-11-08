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
#include QMK_KEYBOARD_H

#define _______ KC_TRNS

enum layers_idx {
    _BL = 0, // Base Layer
    _WL,    // Windows Modifiers Layer (Switched around LGUI, LALT, LCTL)
    _FL,   // Function Layer
    _EL,  // Emoji Layer
    _SL  // Keyboard Settings Layer
};

enum custom_keycodes {
    TOG_OS = BM_SAFE_RANGE
};

enum unicode_names {
    BANG,
    IRNY,
    SNEK
};

const uint32_t PROGMEM unicode_map[] = {
    [BANG]  = 0x203D,  // ‽
    [IRNY] = 0x2E2E,  // ⸮
    [SNEK]  = 0x1F40D, // 🐍
};

#define FN_SPC LT(_FL, KC_SPC)
#define CC_UP LCTL(KC_UP)
#define CC_LEFT LCTL(KC_LEFT)
#define CC_RGHT LCTL(KC_RIGHT)
#define CC_DOWN LCTL(KC_DOWN)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
   [_BL] = LAYOUT(\
  KC_ESC,  KC_GRV,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,     KC_7,    KC_8,    KC_9,    KC_0, KC_MINS,  KC_EQL, KC_BSLS,  KC_DEL, \
   CC_UP,  KC_TAB,      KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,          KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, KC_LBRC, KC_RBRC,   KC_BSPC, \
 CC_DOWN, KC_LGUI,       KC_A,    KC_S,    KC_D,    KC_F,    KC_G,          KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,            KC_ENT, \
 CC_LEFT, KC_LSFT,      KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,         KC_B,    KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH,  KC_RSFT,   CC_RGHT, \
                     KC_LALT, KC_LCTL,           KC_SPC,                         FN_SPC,          KC_RCTL, KC_RALT \
), 
   [_WL] = LAYOUT(\
 _______, _______, _______, _______, _______, _______, _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______, \
 _______, _______,   _______, _______, _______, _______, _______,       _______, _______, _______, _______, _______, _______, _______,   _______, \
 _______, KC_LCTL,    _______, _______, _______, _______, _______,       _______, _______, _______, _______, _______, _______,           _______, \
 _______, _______,   _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______,  _______,   _______, \
                     KC_LGUI, KC_LALT,          _______,                        _______,          KC_RALT, KC_RGUI \
), 
   [_FL] = LAYOUT(\
  BM_RST, _______,   KC_F1,  KC_F2,    KC_F4,   KC_F4,   KC_F5,   KC_F6,    KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,  KC_F12, _______, MO(_EL), \
 _______, _______,   _______,   KC_UP, _______, _______, _______,       _______, _______, _______, _______, _______, _______, _______,   _______, \
 _______, _______,    KC_LEFT, KC_DOWN, KC_RGHT, _______, _______,       KC_LEFT, KC_DOWN,   KC_UP, KC_RGHT, _______, _______,           _______, \
 _______, _______,   _______, _______, _______, _______, _______,       TOG_OS, _______, _______, _______, _______, _______,  _______,   _______, \
                     _______, _______,          MO(_SL),                        _______,          _______, _______ \
), 
   [_EL] = LAYOUT(\
 XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______, \
 XXXXXXX, XXXXXXX,   X(BANG), X(SNEK), X(IRNY), XXXXXXX, XXXXXXX,       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   XXXXXXX, \
 XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,           XXXXXXX, \
 XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,   XXXXXXX, \
                     XXXXXXX, XXXXXXX,          _______,                        _______,          XXXXXXX, XXXXXXX \
), 
   [_SL] = LAYOUT(\
 BM_WIPE, XXXXXXX, BM_HI_1, BM_HI_2, BM_HI_3, BM_HI_4, XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, UC_M_OS, \
 XXXXXXX, XXXXXXX,   BM_HD_1, BM_HD_2, BM_HD_3, BM_HD_4, XXXXXXX,       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   XXXXXXX, \
 XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,           XXXXXXX, \
 XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,   XXXXXXX, \
                     XXXXXXX, XXXXXXX,          _______,                        _______,          XXXXXXX, XXXXXXX \
) };

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
      case TOG_OS: 
        if (record->event.pressed) break;
      switch (biton32(default_layer_state)) {
        case _BL:
          set_single_persistent_default_layer(_WL);
          return false;
        case _WL:
        default:
          set_single_persistent_default_layer(_BL);
          return false;
      }
      break;
    }

    return true;
}

void matrix_init_user(void) {}

void matrix_scan_user(void) {}
