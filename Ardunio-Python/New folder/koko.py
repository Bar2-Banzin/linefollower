import serial
from serial import Serial
import time

print("Start")
port="/dev/tty.HC-06-DevB" #This will be different for various devices and on windows it will probably be a COM port.
bluetooth=serial.Serial('COM7',9600)#Start communications with the bluetooth unit
print("Connected")
bluetooth.flushInput() #This gives the bluetooth a little kick
for i in range(5): #send 5 groups of data to the bluetooth
	print("Ping")
	bluetooth.write(b"BOOP "+str.encode(str(i)))#These need to be bytes not unicode, plus a number
	input_data=bluetooth.readline()#This reads the incoming data. In this particular example it will be the "Hello from Blue" line
	print(input_data.decode())#These are bytes coming in so a decode is needed
	time.sleep(0.1) #A pause between bursts
bluetooth.close() #Otherwise the connection will remain open until a timeout which ties up the /dev/thingamabob
print("Done")
