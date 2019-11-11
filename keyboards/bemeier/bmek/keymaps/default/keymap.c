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
    _OL,  // OS Layer, switching workspaces, media keys, etc.
    _CL  // Keyboard Config Layer
};

enum custom_keycodes {
    TOG_OS = BM_SAFE_RANGE
};

// Space hold function layer
#define FN_SPC LT(_FL, KC_SPC)

// OS-specific shortucts, macOS
#define MN_UP LCTL(KC_UP)
#define MN_LEFT LCTL(KC_LEFT)
#define MN_RGHT LCTL(KC_RGHT)
#define MN_DOWN LCTL(KC_DOWN)
#define MN_SRCH LGUI(KC_SPC)

// Workspace switching Windows - TODO: How to use these on _OL when base layer is _WL?
#define WN_UP LGUI(LCTL(KC_UP))
#define WN_LEFT LGUI(LCTL(KC_LEFT))
#define WN_RGHT LGUI(LCTL(KC_RGHT))
#define WN_DOWN LGUI(LCTL(KC_DOWN))
#define WN_SRCH LGUI(KC_S)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
   [_BL] = LAYOUT(\
 MN_SRCH,  KC_ESC,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,     KC_7,    KC_8,    KC_9,    KC_0, KC_MINS,  KC_EQL, KC_BSLS,  KC_GRV, \
  KC_DEL,  KC_TAB,      KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,          KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, KC_LBRC, KC_RBRC,   KC_BSPC, \
 DM_PLY1, KC_LGUI,       KC_A,    KC_S,    KC_D,    KC_F,    KC_G,          KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,            KC_ENT, \
 DM_PLY2, KC_LSFT,      KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,      MO(_OL),    KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH,  KC_RSFT,   MO(_FL), \
                     KC_LALT, KC_LCTL,           KC_SPC,                         FN_SPC,          KC_RCTL, KC_RALT \
), 
   [_WL] = LAYOUT(\
 WN_SRCH, _______, _______, _______, _______, _______, _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______, \
 _______, _______,   _______, _______, _______, _______, _______,       _______, _______, _______, _______, _______, _______, _______,   _______, \
 _______, KC_LCTL,    _______, _______, _______, _______, _______,       _______, _______, _______, _______, _______, _______,           _______, \
 _______, _______,   _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______,  _______,   _______, \
                     KC_LGUI, KC_LALT,          _______,                        _______,          KC_RALT, KC_RGUI \
), 
   [_FL] = LAYOUT(\
 _______, _______,   KC_F1,  KC_F2,    KC_F4,   KC_F4,   KC_F5,   KC_F6,    KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,  KC_F12,  KC_INS,  KC_DEL, \
 _______, _______,   XXXXXXX,   KC_UP, XXXXXXX, XXXXXXX, XXXXXXX,       KC_PGUP, KC_HOME,   KC_UP,  KC_END, XXXXXXX, KC_UP,   XXXXXXX,   _______, \
 DM_REC1, _______,    KC_LEFT, KC_DOWN, KC_RGHT, XXXXXXX, XXXXXXX,       KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT, KC_LEFT, KC_RGHT,           _______, \
 DM_REC2, _______,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_DOWN,  _______,   _______, \
                     _______, _______,          MO(_CL),                        _______,          _______, _______ \
), 
   [_OL] = LAYOUT(\
 KC_PWR,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
 KC_SLEP, _______,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   XXXXXXX, \
 KC_WAKE, _______,    KC_VOLU, KC_VOLD, KC_MUTE, KC_EJCT, XXXXXXX,         MN_UP, MN_DOWN, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,           XXXXXXX, \
 _______, _______,   KC_MPLY, KC_MPRV, KC_MNXT, XXXXXXX, XXXXXXX,      _______, MN_LEFT, MN_RGHT, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,   _______, \
                     XXXXXXX, XXXXXXX,          XXXXXXX,                        _______,          XXXXXXX, XXXXXXX \
), 
   [_CL] = LAYOUT(\
  BM_RST,  TOG_OS, BM_HI_1, BM_HI_2, BM_HI_3, BM_HI_4, XXXXXXX, XXXXXXX,    BM_SI,   BM_BI,   BM_FI, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
 XXXXXXX, XXXXXXX,   BM_HD_1, BM_HD_2, BM_HD_3, BM_HD_4, XXXXXXX,         BM_SD,   BM_BD,   BM_FD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   BM_WIPE, \
 XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,           XXXXXXX, \
 XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,   _______, \
                     XXXXXXX, XXXXXXX,          _______,                        _______,          XXXXXXX, XXXXXXX \
) };

// Handling default layers is done here in the _user function.
// In the _kb functions (see bmek.c) the BM_ keycodes are handled.
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
