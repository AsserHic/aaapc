#!/usr/bin/env python

from arduino_connection import ArduinoConnection
from lcd_provider import LCDProvider

arduino = ArduinoConnection()
lcd = LCDProvider()

keep_alive = True
while keep_alive:
    operation, args = arduino.read_response()
    lcd.show_text('{}: {}'.format(operation, args))
