import serial

port_name = "COM6" # Port the MCU is plugged into
baud_rate = 9600 # Same as MCU

# Create a Serial class with the port and baud rate that stops reading after 1 seconds (when calling read())
port = serial.Serial(port_name, baud_rate, timeout=1)

# Wait until data has been sent from the device
while True:
    if port.in_waiting > 0:
        data = port.read()
        if data == 'b':
            # Play alarm sound
            print("BRRRRRRRRRRRRRRRRRRRRRiinnnggggg~~~")
        elif data == 's':
            # Stop alarm from ringing
            print("Alarm stopped")
        else:
            print("Placeholder if it sends anything else")
