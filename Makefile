#
# Author: Armando Califano
# cc Armando Califano
# Purpose: Makefile that includes necessary flags to comiple a program
#	   implementing the OpenCV library

CPPFLAGS = `pkg-config --cflags opencv`
LDLIBS = `pkg-config --libs opencv`
