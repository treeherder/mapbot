#!/usr/local/bin/python
from subprocess import call
import argparse

p = argparse.ArgumentParser(description='capture an image with a name')
p.add_argument('--image', dest='image', metavar='i', help='give a name to save the image as' )
args = p.parse_args()
call(['raspistill', '-o', 'map_pics/{0}'.format(args.image)])
print('map_pics/{0}'.format(args.image))
