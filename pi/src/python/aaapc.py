#!/usr/bin/env python

import time

from arduino_connection import ArduinoConnection
from lcd_provider import LCDProvider

arduino = ArduinoConnection()

lcd = LCDProvider()

keep_alive = True
while keep_alive:
    arduino.send_request('temperature')
    time.sleep(4)

    if arduino.available():
       operation, args = arduino.read_response()
       lcd.show_text('{}: {}'.format(operation, args))
