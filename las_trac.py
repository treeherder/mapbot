#!/usr/local/bin/python
import cv2
import numpy as np
from subprocess import call
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
    # set up media and capture an image:
    call(['python', 'stilpy.py', '--image', '{0}'.format(im)])
  def las_cap(self, im):
    chassis.las_on()
    self.cap(im)
    chassis.las_off()

def basic():

  success, raw = captured.read() # a successs flag and the image array
  smooth_img = cv2.blur(raw,(2,2))    # filter out some complexity
  hsv_img = cv2.cvtColor(smooth_img, cv2.cv.CV_RGB2HSV)  # translate to HSV
  (hue, saturation, value) = cv2.split(hsv_img)
  cv2.cv.InRange(hue,5,6,hue)
  cv2.cv.InRange(saturation,50,100,saturation)
  cv2.cv.InRange(value,250,256,value)  

  '''display results'''
  #cv2.imshow('hsv space', hsv_img)
  #cv2.imshow('hue space', hue)
  #cv2.imshow('saturation  space', saturation)
  #cv2.imshow('value space', value)


  #cv2.imshow(' smoooth', smooth_img)
  #cv2.imshow('raw video', raw)
chassis = Chassis()

x = time.asctime().replace(' ' , '')
x = x.replace(':','')
x+="-{0}".format(chassis.compass())
x = x.replace('.','_')
x=x[:30]
trial = imager()
trial.las_cap(x)

