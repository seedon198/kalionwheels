import tkinter as tk  # import tkinter for GUI
import smbus  # import smbus for I2C communication
import time  # import time for sleep function
import os  # import os module
 
#------- declare variables
slave = 8  # Arduino UNO's I2C Address
array1 = [0x7F, 0x7F, 0x7F, 0x7F ]  # init array1 to store byte values to be sent to the Arduino UNO over I2C
commandByte = 0x01  # commandbyte, of no use in this case but required for sending I2C data
bus = smbus.SMBus(1)    # 0 = port I2C0, 1 = port I2C1
 
 
# Function to handle I2C data transfer
def transferarray():
    commandByte = 0x00
    bus.write_i2c_block_data(slave, commandByte, array1)   # Write array1 to the i2c bus
    print (array1) 
#     commandByte = 0x01
#     bus.write_i2c_block_data(slave, commandByte, array1) #Write an array to the i2c bus
#     time.sleep(0.5)
 
# one byte is used to control the motion of each motor
# lets say array1[0] is equal to 0x00 or unsigned integer 0
# in that case the motor is set to turn back in the arduino code
# in case array1[0] is equal to 0xFF or unsigned integer 255
# the motor will turn forward and for any other value the motor will 
# stop moving.
# functions below will update relevent values to the array 
# if specific keys are pressed.


def fwd():
    array1[0] = 0xFF
    print("forward\n")
    transferarray()
 
 
def bwd():
    array1[0] = 0x00
    print("back\n")
    transferarray()
 
 
def left():
    array1[1] = 0xFF
    print("left\n")
    transferarray()
 
 
def right():
    array1[1] = 0x00
    print("right\n")
    transferarray()
 
 
def stop():
    array1[0] = 0x7F
    array1[1] = 0x7F
    print("stop\n")
    transferarray()
 
 
def panleft():
    if array1[2] <= 0xEF:
        array1[2] = array1[2] + 0x03
    else:
        array1[2] = 0xFF
    print("panleft\n")
    transferarray()
 
 
def panright():
    if array1[2] >= 0x1F:
        array1[2] = array1[2] - 0x03
    else:
        array1[2] = 0x0F
    print("panright\n")
    transferarray()
 
 
def tiltup():
    if array1[3] <= 0xEF:
        array1[3] = array1[3] + 0x03
    else:
        array1[3] = 0xFF
    print("tiltup\n")
    transferarray()
 
 
def tiltdown():
    if array1[3] >= 0x1F:
        array1[3] = array1[3] - 0x03
    else:
        array1[3] = 0x0F
    print("tiltdown\n")
    transferarray()


def rest():
    array1[3] = 0x7F
    array1[2] = 0x7F
    print("reset camera\n")
    transferarray()
 

# Function to handle key press
# Keys are assigned as follows
# w,a,s,d  move the robot forward, back, left and right
# spacebar for brakes
# q & e to pan the camera left & right
# r & t to pan the camera up & down
# z to reset the camera position to default i.e. 90 degrees, range of the servo is 180 degrees
# y to nod yes with the servo mechanism
# n to nod no with the servo mechanism


def key_input(event):
    print('Key:', event.char)
    key_press = event.char
 
    if key_press.lower() == 'w':
        fwd()
    elif key_press.lower() == 's':
        bwd()
    elif key_press.lower() == 'a':
        left()
    elif key_press.lower() == 'd':
        right()
    elif key_press.lower() == 'q':
        panleft()
    elif key_press.lower() == 'e':
        panright()
    elif key_press.lower() == 'r':
        tiltup()
    elif key_press.lower() == 'f':
        tiltdown()
    elif key_press.lower() == ' ':
        stop()
    elif key_press.lower() == 'z':
        rest()
 
 
command = tk.Tk()  # Tkinter Window
command.bind_all('<KeyPress>', key_input)  # capture key press
