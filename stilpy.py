#!/usr/local/bin/python
from subprocess import call
import argparse

p = argparse.ArgumentParser(description='capture an image with a name')
p.add_argument('--image', dest='image', metavar='i', type=str,  help='give a name to save the image as' )
args = p.parse_args()


call(['raspistill', '-o', '{0}.jpg'.format(args.image)])
