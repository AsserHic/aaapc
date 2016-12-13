#!/usr/bin/env python

import random

from arduino_connection import *
from lcd_provider import LCDProvider

def mode_led_blue(activate):
    arduino.send_request(OPER_LED_BLUE, (1 if activate else 0))

def mode_led_yellow(activate):
    arduino.send_request(OPER_LED_YELLOW, (1 if activate else 0))

def mode_led_white(activate):
    arduino.send_request(OPER_LED_WHITE, (1 if activate else 0))

MODES = [
    mode_led_blue,
    mode_led_white,
    mode_led_yellow,
]
currentMode = 0

def changeMode(old_mode, adjustment):
    new_mode = new_mode+adjustment
    if new_mode < 0:
        new_mode = len(MODES)-1
    elif new_mode >= len(MODES):
        new_mode = 0
    MODES[old_mode](False)
    MODES[new_mode](True)
    return new_mode

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

    if arduino.available():
       operation, args = arduino.read_response()
       if operation in TO_LCD_OPS:
          lcd.show_text('{}: {}'.format(operation, args))
       elif operation == OPER_JOYSTICK:
          args = [ int(x) for x in args.split(',') ]
          if args[0] < -98:
              currentMode = changeMode(currentMode, -1)
          elif args[0] > 98:
              currentMode = changeMode(currentMode, 1)
       elif operation == OPER_ERROR:
          print('arduino error: {}'.format(args))
       else:
          print('Unexpected operation {}: {}.'.format(operation, args))

exit()
