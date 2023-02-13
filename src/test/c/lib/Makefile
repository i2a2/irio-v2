#+======================================================================
# $HeadURL: 
# $Id: 
#
# Project       : IRIO Driver
#
# Description   : IRIO Driver library test Makefile
#
# Authors       : Mariano Ruiz, Diego Sanz, Sergio Esquembri, Enrique Bernal, Alvaro Bustos
# Authors Affiliation    : Universidad Politécnica de Madrid (UPM)
#
# Copyright (C) : 2010-2015 Universidad Politécnica de Madrid (UPM)
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
#
#-======================================================================

NIRIO = /usr

TARGET=../../..
SRC=../../..
COVERAGE=true

LIBRARIES=NiFpga niflexrio irioCore pthread gtest 
LIBRARY_DIRS=$(TARGET)/lib
INCLUDE_DIRS=$(SRC)/main/c/include

ifdef CODAC_ROOT
	INCLUDE_DIRS+=$(CODAC_ROOT)/include
else
	LIBRARY_DIRS+=$(NIRIO)/lib/x86_64-linux-gnu
	INCLUDE_DIRS+=$(SRC)/main/c/support
endif

INCLUDES=$(foreach inc,$(INCLUDE_DIRS),-I$(inc))
LDLIBS=$(foreach libs,$(LIBRARY_DIRS),-L$(libs) -Wl,-rpath,$(libs)) $(foreach libs,$(LIBRARIES),-l$(libs))

C=gcc
CC=g++
CFLAGS=-c -Wall
CCFLAGS=-c -Wall -std=c++11
LDFLAGS=

ifeq ($(COVERAGE),true)
	CFLAGS+=-O0 -g
	CCFLAGS+=-O0 -g
	LDFLAGS+=--coverage
endif

SOURCES=$(wildcard *.c)
TESTS=$(SOURCES:.c=)

.PHONY : all test clean runner runner.cpp

all: runner $(TESTS)

# A rule that runs the unit tests
test: runner

# A rule that cleans everything
clean:
	rm -f runner $(TESTS)

# How to build the test runner
runner: irioCorelib-test.cpp
	$(CC) $(CCFLAGS) $(LDFLAGS) $(LDLIBS) $(INCLUDES) -o $@ $^

%: %.c
	$(C) $(CFLAGS) $(LDFLAGS) $(INCLUDES) -o $@ $<

