## ======================================================================
## Makefile
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

include mk/conf-gnu.mk 

LIBS += -lX11 -lpthread

ipath += -I./include -I./libs -I./xlibs

CFLAGS += $(shell pkg-config --cflags xrender freetype2)
LDFLAGS += -lfreetype -lXrender -lXft -lm

src   = src
libs  = libs
xlibs = xlibs

MAIN = solar-stuff
bin = ./bin

SRC1 = $(src)/$(MAIN).c
SRC2 = $(xlibs)/Xhelper.c $(xlibs)/Screen.c
SRC3 = $(libs)/solar-infos.c $(libs)/clist.c
SRC  = $(SRC1)
SRC	+= $(SRC2)
SRC += $(SRC3)

_objs1 = $(SRC1:.c=.o)
_objs2 = $(SRC2:.c=.o)
_objs3 = $(SRC3:.c=.o)

OBJS  = $(_objs1:$(src)/%=$(odir)/%)
OBJS += $(_objs2:$(xlibs)/%=$(odir)/%)
OBJS += $(_objs3:$(libs)/%=$(odir)/%)

include mk/targ-gnu.mk 
