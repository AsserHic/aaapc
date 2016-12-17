#!/usr/bin/env python

import logging
import random
from time import sleep

from arduino_connection import *
from lcd_provider import LCDProvider

def mode_distance(activate):
    if activate:
        arduino.send_request(OPER_DISTANCE)

def mode_temperature(activate):
    if activate:
        arduino.send_request(OPER_TEMPERATURE)

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
    mode_distance,
    mode_temperature,
]
currentMode = 0

def changeMode(old_mode, adjustment):
    new_mode = old_mode+adjustment
    if new_mode < 0:
        new_mode = len(MODES)-1
    elif new_mode >= len(MODES):
        new_mode = 0
    MODES[old_mode](False)
    MODES[new_mode](True)
    return new_mode

arduino = ArduinoConnection()

lcd = LCDProvider()

keep_alive = True
while keep_alive:
    #arduino.send_request(OPER_RGB_LED, [random.randint(0, 150) for i in range(3)])

    while arduino.available():
       operation, args = arduino.read_response()
       if operation == OPER_JOYSTICK:
          args = [ int(x) for x in args.split(',') ]
          if args[0] < -99:
              currentMode = changeMode(currentMode, -1)
          elif args[0] > 99:
              currentMode = changeMode(currentMode, 1)
       elif operation == OPER_DISTANCE:
           lcd.show_text('dist {}cm'.format(args))
           if int(args) < 10:
               arduino.send_request(OPER_BUZZLER, [2000, 100])
       elif operation == OPER_TEMPERATURE:
           lcd.show_text('temp {}C'.format(args))
       elif operation == OPER_ERROR:
          logging.error('arduino error: {}'.format(args))
       else:
          logging.error('Unexpected operation {}: {}.'.format(operation, args))
    sleep(0.1)

exit()
