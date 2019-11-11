MCU = atmega32u4
BOOTLOADER = atmel-dfu
BOOTMAGIC_ENABLE = no      # Virtual DIP switch configuration(+1000)
MOUSEKEY_ENABLE = no       # Mouse keys(+4700)
EXTRAKEY_ENABLE = yes      # Audio control and System control(+450)
CONSOLE_ENABLE = no        # Console for debug(+400)
COMMAND_ENABLE = no        # Commands for debug and configuration
SLEEP_LED_ENABLE = no       # Breathing sleep LED during USB suspend
SPLIT_KEYBOARD = no         # split keyboard
BACKLIGHT_ENABLE = no       # Enable keyboard backlight functionality
NKRO_ENABLE = yes           # USB Nkey Rollover
RGBLIGHT_ENABLE = yes       # Enable keyboard RGB underglow
MIDI_ENABLE = no            # MIDI support (+2400 to 4200, depending on config)
UNICODE_ENABLE = no         # Unicode
UNICODEMAP_ENABLE = no      # Unicode Map
BLUETOOTH_ENABLE = no       # Enable Bluetooth with the Adafruit EZ-Key HID
AUDIO_ENABLE = no           # Audio output on port C6
FAUXCLICKY_ENABLE = no      # Use buzzer to emulate clicky switches
HD44780_ENABLE = no 		# Enable support for HD44780 based LCDs (+400)
DYNAMIC_MACRO_ENABLE = yes  # Macro keys
