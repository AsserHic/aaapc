import RPi.GPIO as GPIO
from RPLCD import CharLCD, BacklightMode

lcd = CharLCD(pin_rs=32, pin_e=40, pins_data=[29, 31, 11, 12],
              cols=16, rows=2, dotsize=8,
              numbering_mode    = GPIO.BOARD,
              auto_linebreaks   = True,
              pin_backlight     = None,
              backlight_enabled = True,
              backlight_mode    = BacklightMode.active_low)

lcd.write_string('Hello')

# If desired, reset the GPIO configuration and optionally clear the screen.
# Note that this can lead to undesired effects on the LCD, because the GPIO
# pins are not configured as input or output anymore.
#lcd.close(clear=True)
