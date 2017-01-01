#!/usr/bin/env python

import logging
import random
from time import sleep

from arduino_connection import *
from lcd_provider import LCDProvider

logging.basicConfig(level = logging.INFO, format = '%(levelname)s: %(message)s')

def mode_light_sensor(activate):
    if activate:
        arduino.send_request(OPER_LIGHT_SENSOR1)
        arduino.send_request(OPER_LIGHT_SENSOR2)

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
    mode_light_sensor,
]
currentMode = 0

def changeMode(old_mode, adjustment):
    assert adjustment == -1 or adjustment == 1
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

light_sensors = [0, 0]

keep_alive = True
while keep_alive:
    while arduino.available():
       operation, args = arduino.read_response()
       if operation == OPER_JOYSTICK:
          args = [ int(x) for x in args.split(',') ]
          if args[0] < -98:
              if args[1] < -98 and args[2] == 1:
                  keep_alive = False
              currentMode = changeMode(currentMode, -1)
          elif args[0] > 98:
              currentMode = changeMode(currentMode, 1)
       elif operation == OPER_DISTANCE:
           lcd.show_text('dist {}cm'.format(args))
           if int(args) < 10:
               arduino.send_request(OPER_BUZZLER, [2000, 100])
       elif operation == OPER_TEMPERATURE:
           lcd.show_text('temp {}C'.format(args))
       elif operation == OPER_LIGHT_SENSOR1:
           light_sensors[0] = int(args)
       elif operation == OPER_LIGHT_SENSOR2:
           light_sensors[1] = int(args)
           lcd.show_text('* {} {}'.format(light_sensors[0],
                                          light_sensors[1]))
       elif operation == OPER_HUMAN_DETECTOR:
           if args == '1':
              color = [random.randint(0, 5)*50 for i in range(3)]
           else:
              color = '0,0,0'
           arduino.send_request(OPER_RGB_LED, color)
       elif operation == OPER_ERROR:
          logging.error('arduino error: {}'.format(args))
       else:
          logging.error('Unexpected operation {}: {}.'.format(operation, args))
    sleep(0.1)

logging.info('Bye bye!')
exit()
