#!/usr/local/bin/python
import cv2
import numpy as np
from subprocess import call
import time

def cap(im):
  # set up media and capture an image:
  call(['python', 'stilpy.py', '--image', '{0}'.format(im)])

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
x = time.asctime().replace(' ' , '')
x = x.replace(':','')
cap(x)
