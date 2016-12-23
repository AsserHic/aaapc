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
            print(text)
            return OPER_ERROR, 'Invalid response'
        text      = text[0:(len(text)-2)]
        op_delim  = text.index(':', 1)
        operation = text[1:op_delim]
        operation = int(operation)
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
