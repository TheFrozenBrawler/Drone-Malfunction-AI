import serial
### config for drone_malfunction_ai

NB_OF_MEAS  = 10     # number of measurements

# serial config
PORT        = "COM10"
BAUDRATE    = 115200
BYTESIZE    = serial.EIGHTBITS
PARITY      = serial.PARITY_NONE
STOPBITS    = serial.STOPBITS_ONE
TIMEOUT     = 1

# file system
FILE_NAME = 'one_wing_vibr'