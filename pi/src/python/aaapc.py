#!/usr/bin/env python

import random
import time

from arduino_connection import *
from lcd_provider import LCDProvider

arduino = ArduinoConnection()

lcd = LCDProvider()

TO_LCD_OPS = [
  OPER_DISTANCE,
  OPER_LIGHT_SENSOR1,
  OPER_LIGHT_SENSOR2,
  OPER_TEMPERATURE,
]

keep_alive = True
while keep_alive:
    arduino.send_request(OPER_RGB_LED, [random.randint(0, 150) for i in range(3)])
    #arduino.send_request(OPER_BUZZLER, [8000, 150])
    arduino.send_request(OPER_TEMPERATURE)
    #arduino.send_request(OPER_DISTANCE)

    if arduino.available():
       operation, args = arduino.read_response()
       if operation in TO_LCD_OPS:
          lcd.show_text('{}: {}'.format(operation, args))
       elif operation == OPER_ERROR:
          print('arduino error: {}'.format(args))
       else:
          print('Unexpected operation {}: {}.'.format(operation, args))
    time.sleep(4)
