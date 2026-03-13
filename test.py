import serial, time
ser = serial.Serial("COM11", 9600, timeout=1)
print("should print")
time.sleep(1)     # wait for auto-reset to finish
ser.write(5)
ser.close()