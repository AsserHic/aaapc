import serial

serial_connection = serial.Serial('/dev/ttyACM0', 9600)

while 1:
  input_line = serial_connection.readline()
  print(input_line)
