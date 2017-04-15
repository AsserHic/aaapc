import logging
import serial

OPER_ERROR          = 1
OPER_RGB_LED        = 10
OPER_BUZZLER        = 15
OPER_LED_BLUE       = 21
OPER_LED_WHITE      = 22
OPER_LED_YELLOW     = 23
OPER_LIGHT_SENSOR1  = 30
OPER_LIGHT_SENSOR2  = 31
OPER_TEMPERATURE    = 33
OPER_DISTANCE       = 35
OPER_HUMAN_DETECTOR = 36
OPER_JOYSTICK       = 38
OPER_DISPLAY_SEQ    = 40

DISPLAY_CHARS = {
  ' ': 0b00000000,
  '0': 0b00111111,
  '1': 0b00000110,
  '2': 0b01011011,
  '3': 0b01001111,
  '4': 0b01100110,
  '5': 0b01101101,
  '6': 0b01111101,
  '7': 0b00000111,
  '8': 0b01111111,
  '9': 0b01101111,
  '.': 0b10000000,
  '-': 0b01000000,
  '^': 0b00000001,
  '"': 0b00100010,
  '_': 0b00001000,
  'A': 0b01110111,
  'b': 0b01111100,
  'C': 0b00111001,
  'c': 0b01011000,
  'd': 0b01011110,
  'E': 0b01111001,
  'e': 0b01111011,
  'F': 0b01110001,
  'G': 0b00111101,
  'H': 0b01110110,
  'h': 0b01110100,
  'I': 0b00110000,
  'J': 0b00011110,
  'j': 0b00001110,
  'L': 0b00111000,
  'o': 0b01011100,
  'P': 0b01110011,
  'Q': 0b10111111,
  'r': 0b01110000,
  'S': 0b01101101,
  'T': 0b00000111,
  'U': 0b00111110,
  'u': 0b00011100,
  'Y': 0b01110010,
  'y': 0b01100110,
}

class ArduinoConnection(object):

    def __init__(self, tty='/dev/ttyACM0', baud_rate=9600):
        self.serial_con = serial.Serial(tty, baud_rate)
        self.serial_con.flush()
        while self.serial_con.inWaiting():
           self.serial_con.read()

    def available(self):
        return self.serial_con.inWaiting()

    def _readline(self):
        line = self.serial_con.readline()
        return line

    def read_response(self):
        text = self._readline()
        if text[0] != '*':
            logging.error(text)
            return OPER_ERROR, 'Invalid response start'
        try:
            text      = text[0:(len(text)-2)]
            op_delim  = text.index(':', 1)
            operation = text[1:op_delim]
            operation = int(operation)
        except ValueError:
            logging.error(text)
            return OPER_ERROR, 'Invalid response code'
        if len(text) > op_delim+1:
            args = text[(op_delim+1):]
        else:
            args = None
        return operation, args

    def send_request(self, operation, args = None):
        if args is None:
           args = ''
        elif isinstance(args, list):
           args = ','.join(str(x) for x in args)
        message = '*{}:{}\n'.format(operation, args)
        self.serial_con.write(message)

    def set_display_sequence(self, text):
        assert len(text) <= 150

        args = [ len(text) ]
        for c in text:
            args.append(DISPLAY_CHARS.get(c, 0))
        self.send_request(OPER_DISPLAY_SEQ, args)
