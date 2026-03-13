import serial
import pyfiglet
import time

# Print the name and stuff 
logo = pyfiglet.figlet_format("Flappy    Penguins", font="big")
slogan = "The cutest, most fluffy penguin that wakes you up"

print("\n\n\n")
print(logo)
print(slogan)
print("\n\n\n")

wait_time = 0

print("(For demonstration purposes, the time is limited to 255 seconds)")
while True:
    try:
        wait_time = int(input("Please enter a number of seconds to wait for: "))
        if (wait_time >= 0 and wait_time < 255):
            break
        print("\nPlease enter a value between 0 and 255 seconds")
    except ValueError:
        print("\nPlease enter a number")

print(f"\nSending {wait_time} second(s) to the microncontroller...\n\n\n")

#port_name = "COM11" # Port the MCU is plugged into
#baud_rate = 9600 # Same as MCU

# Create a Serial class with the port and baud rate that stops reading after 1 seconds (when calling read())
#port = serial.Serial(port_name, baud_rate, timeout=1)
#time.sleep(1.0)

#port.write(wait_time)

zzz = pyfiglet.figlet_format("Good night   ...", font="slant")
print(zzz)

#port.close()

# import serial, time
# ser = serial.Serial("COM11", 9600, timeout=1)
# print("should print")
# time.sleep(1)     # wait for auto-reset to finish
# ser.write(10)
# ser.close()