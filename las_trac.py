#!/usr/local/bin/python
import cv2
import numpy as np
from subprocess import Popen, PIPE
import time
import serial

class ctrl:
	PING = "5"
	COMPASS = "6"
	LASER_ON = "7"
	LASER_OFF = "8"
	
class Chassis():
 
  def __init__(self):
    self.com = serial.Serial('/dev/ttyAMA0', '9600', timeout = None)
    time.sleep(2)
 
  def ping(self):
    self.com.write(ctrl.PING) # command for ping data
    self.com.flush()
    time.sleep(0.2)
    return  self.com.readline()
 
 
  def compass(self):
    self.com.write(ctrl.COMPASS)  # command for compass data
    time.sleep(0.2)
    return  self.com.readline()

    self.com.flush()
    
  def las_on(self):
    self.com.write(ctrl.LASER_ON)  # toggle laser
    self.com.flush()
    
  def las_off(self):
    self.com.write(ctrl.LASER_OFF)  # toggle laser
    self.com.flush()
    
class imager():
  def cap(self, im):
    chassis.las_on()
    process = Popen(['python', 'stilpy.py', '--image', '{0}'.format(im)], stdout=PIPE)
    value, err  = process.communicate() 
    print(value)
    chassis.las_off()


def basic(z):
  img = cv2.imread('map_pics/{0}'.format(z))
  hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
  cv2.imwrite("map_pics/hsv{0}".format(z), hsv)
 
chassis = Chassis()

x = time.asctime().replace(' ' , '')
x = x.replace(':','')
x+="-{0}".format(chassis.compass())
x = x.replace('.','_')
x=x[:25]
x+='.jpg'
trial = imager()
trial.cap(x)
basic(x)
