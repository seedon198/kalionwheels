import tkinter as tk
import smbus
import time
import os
 
#------- declare variables
slave = 8
array1 = [0x7F, 0x7F, 0x7F, 0x7F ]
commandByte = 0x01
bus = smbus.SMBus(1)    # 0 = port I2C0, 1 = port I2C1
 
 
def transferarray():
    commandByte = 0x00
    bus.write_i2c_block_data(slave, commandByte, array1)
    print (array1) #Write an array to the i2c bus
#     commandByte = 0x01
#     bus.write_i2c_block_data(slave, commandByte, array1) #Write an array to the i2c bus
#     time.sleep(0.5)
 
 
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
 
 
command = tk.Tk()
command.bind_all('<KeyPress>', key_input)
