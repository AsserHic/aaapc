import RPi.GPIO as GPIO
from RPLCD import CharLCD, BacklightMode

class LCDProvider(object):

    def __init__(self):
		self.line1 = ''
		self.line2 = ''

        GPIO.setwarnings(False)
        self.lcd = CharLCD(pin_rs=32, pin_e=40, pins_data=[29, 31, 11, 12],
                           cols=16, rows=2, dotsize=8,
                           numbering_mode    = GPIO.BOARD,
                           auto_linebreaks   = True,
                           pin_backlight     = None,
                           backlight_enabled = True,
                           backlight_mode    = BacklightMode.active_low)
        self.lcd.clear()

    def _update_display():
		text = line1+'\n'+line2
		self.lcd.write_string(text)

    def show_text(self, text):
        text = text[:16]
        l    = len(text)
        if l < 16:
			text = text+(' '*(16-l))
	    self.line1 = self.line2
	    self.line2 = text
