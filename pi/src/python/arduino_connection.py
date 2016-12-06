import serial

OPERATIONS = {
  'distance':      'RD',
  'led_blue':      'Lb',
  'led_white':     'Lw',
  'led_yellow':    'Ly',
  'light_sensor1': 'P1',
  'light_sensor2': 'P2',
  'rgb_led':       'Lx',
  'sound':         'BZ',
  'temperature':   'RT',
}

class ArduinoConnection(object):

    def __init__(self, tty='/dev/ttyACM0', baud_rate=9600):
        self.serial_con = serial.Serial(tty, baud_rate)
        self.serial_con.flush()

    def available(self):
        return self.serial_con.inWaiting()

    def _readline(self):
        line = self.serial_con.readline()
        return line

    def read_response(self):
        text      = self._readline()
        operation = text[0:2]
        if len(text) > 3:
            args = text[2:(len(text)-1)]
        else:
            args = None
        return operation, args

    def send_request(self, operation, args = None):
        oper_code = OPERATIONS[operation]
        if args is None:
           args = ''
        elif isinstance(args, list):
           args = ','.join(str(x) for x in args)
        message = '{}{}\n'.format(oper_code, args)
        self.serial_con.write(message)
