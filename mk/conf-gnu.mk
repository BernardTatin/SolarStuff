## ======================================================================
## conf-gnu.mk
## SolarStuff project
## Author: Bernard TATIN <bernard dot tatin at outlook dot org>
##
## Created on 3 mars 2016, 22:20
##
## ======================================================================

##    The MIT License (MIT)
##
##    Copyright (c) 2016 Bernard TATIN <bernard dot tatin at outlook dot org>
##
##    Permission is hereby granted, free of charge, to any person obtaining a copy
##    of this software and associated documentation files (the "Software"), to deal
##    in the Software without restriction, including without limitation the rights
##    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
##    copies of the Software, and to permit persons to whom the Software is
##    furnished to do so, subject to the following conditions:
##
##    The above copyright notice and this permission notice shall be included in all
##    copies or substantial portions of the Software.
##
##    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
##    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
##    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
##    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
##    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
##    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
##    SOFTWARE.

os := $(shell uname)
arch ?= -m64
std=c11

ifeq ($(os), SunOS)
	CC ?= cc
	optim ?= -xO3
	warns  = -errtags=yes
	ipath  = 
else ifeq ($(os), Linux)
	CC = clang
	LIBS = -L/usr/lib -lX11
	optim ?= -O2
	warns  = -Wall -pedantic
	ipath  = -I/usr/include
else ifeq ($(os), FreeBSD)
	CC = gcc
	LIBS = -L/usr/local/lib
	optim ?= -g2
	warns  = -Wall -pedantic
	ipath  = -I/usr/local/include
endif

CFLAGS += -std=$(std)
CFLAGS += $(optim) $(warns) $(ipath) $(arch)
CFLAGS += -D_REENTRANT

LDFLAGS += $(arch)

LD = $(CC)
RM = rm -v -f

odir  = objs$(arch)