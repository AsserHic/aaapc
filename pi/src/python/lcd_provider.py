import RPi.GPIO as GPIO
from RPLCD import CharLCD, cleared, cursor

class LCDProvider(object):

    def __init__(self):
        self.line1 = 'Hello World!'
        self.line2 = ''

        GPIO.setwarnings(False)
        self.lcd = CharLCD(pin_rs=32, pin_e=40, pins_data=[29, 31, 11, 12],
                           cols=16, rows=2, dotsize=8,
                           numbering_mode    = GPIO.BOARD,
                           auto_linebreaks   = False,
                           pin_backlight     = None,
                           backlight_enabled = True)
        self._update_display()

    def _update_display(self):
        with cleared(self.lcd):
             self.lcd.write_string(self.line1)
        with cursor(self.lcd, 1, 0):
             self.lcd.write_string(self.line2)

    def show_text(self, msg):
	self.line1 = self.line2
	self.line2 = msg
        self._update_display()
