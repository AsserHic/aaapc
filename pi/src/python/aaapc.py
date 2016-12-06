#!/usr/bin/env python

import random
import time

from arduino_connection import ArduinoConnection
from lcd_provider import LCDProvider

arduino = ArduinoConnection()

lcd = LCDProvider()

keep_alive = True
while keep_alive:
    arduino.send_request('rgb_led', [random.randint(0, 200) for i in range(3)])
    arduino.send_request('sound', [8000, 200])
    #arduino.send_request('temperature')
    arduino.send_request('distance')
    time.sleep(4)

    if arduino.available():
       operation, args = arduino.read_response()
       lcd.show_text('{}: {}'.format(operation, args))
