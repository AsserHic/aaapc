import serial

class ArduinoConnection(object):

    OPERATIONS = {
      'temperature':   'RT',
      'light_sensor1': 'P1',
      'light_sensor2': 'P2',
      'led_white':     'Lw',
      'led_yellow':    'Ly',
      'led_blue':      'Lb',
    }

    def __init__(self, tty='/dev/ttyACM0', baud_rate=9600):
        self.serial_con = serial.Serial(tty, baud_rate)

    def _readline(self):
        line = serial_con.readline()
        return line

    def read_response(self):
        text = self._readline()
        text = text.split(',', 1)
        operation = text[0]
        if len(text) > 1:
            args = text[1]
        else:
            args = None
        return operation, args

    def send_request(self, operation, args = None):
        oper_code = OPERATIONS[operation]
        if args is None:
           args = ''
        message = '{}{}\n'.format(oper_code, args)
        self.serial_con.write(message)
