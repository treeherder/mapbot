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
  (hue, sat, val)  = cv2.split(hsv)
  hf, hue  = cv2.threshold(hue, 0, 20, cv2.THRESH_BINARY)
  sf, sat  = cv2.threshold(sat, 254, 255, cv2.THRESH_BINARY)
  vf, val  = cv2.threshold(val, 200, 220, cv2.THRESH_BINARY)
  mrgd = cv2.merge([hue, sat, val])
  bgr = cv2.cvtColor(mrgd, cv2.COLOR_HSV2BGR)
  gray = cv2.cvtColor(bgr, cv2.COLOR_BGR2GRAY)
  cv2.imwrite("map_pics/h-{0}".format(z), gray)

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
