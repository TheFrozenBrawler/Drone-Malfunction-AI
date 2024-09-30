import config as CFG

import serial
import os

ser = serial.Serial(port=CFG.PORT, 
                    baudrate=CFG.BAUDRATE, 
                    bytesize=CFG.BYTESIZE,
                    parity=CFG.PARITY,
                    stopbits=CFG.STOPBITS)

byte_msg_lines = []
byte_object = []
byte_char = []

print("READY to read data from ", ser.name, " port")

# get data from STM324
for i in range (CFG.NB_OF_MEAS):
    while (1):
        ''' Reading one character at a time into a byte_object. Checking if the ';' character, 
            which signifies the end of the object, is present.
            If so, checking whether the object is a newline symbol or a message end symbol. 
            If so, saving the entire line to byte_msg_lines '''
        
        byte = ser.read(1)
        if (byte == b';'):
            byte_char.append(byte)
            byte_msg = b''.join(byte_char)
            
            byte_char.clear()

            if byte_msg == b"line;":
                byte_msg_lines.append( b''.join(byte_object) )
                byte_object.clear()
            elif byte_msg == b"end;":
                break
            else:
                byte_object.append(byte_msg)
                
        else:
            byte_char.append(byte)


    # write to file
    file_name = CFG.FILE_NAME + "_" + str(i) + ".csv"
    
    if (os.path.exists(file_name)):
        os.remove(file_name)
    
    with open(file_name, 'w', newline='') as file:

        # file.write("Tp;AC1;AC2;AC3;\n") # write headers
        
        for i in range( len(byte_msg_lines) ):
            file.write( byte_msg_lines[i].decode('utf-8') )
            file.write("\n")

    file.close()
    byte_msg_lines.clear()
    byte_object.clear()
    byte_char.clear()

ser.close()

print("program ended")
