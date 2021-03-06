#+======================================================================
# $HeadURL$
# $Id$
#
# Project       : CODAC Core System
#
# Description   : $progName Makefile
#
# Author        : This file was generated by CODAC development toolkit
#
# Copyright (c) : 2010-2022 ITER Organization,
#                 CS 90 046
#                 13067 St. Paul-lez-Durance Cedex
#                 France
#
# This file is part of ITER CODAC software.
# For the terms and conditions of redistribution or use of this software
# refer to the file ITER-LICENSE.TXT located in the top level directory
# of the distribution package.
#
#-======================================================================

PROGNAME=driver-irio
TARGET=../../../../target

LIBRARIES=irioCore pthread rt gtest
LIBRARY_DIRS=$(TARGET)/lib
INCLUDE_DIRS=$(TARGET)/main/c/include $(TARGET)/main/c/support $(CODAC_ROOT)/include ../include

BINARY_DIR=.
SOURCE_DIR=.
OBJECT_DIR = $(SOURCE_DIR)/.obj

EXECUTABLE=$(BINARY_DIR)/$(PROGNAME)
INCLUDES=$(foreach inc,$(INCLUDE_DIRS),-I$(inc))
#LDLIBS=-L$(CODAC_ROOT)/lib  $(foreach libs,$(LIBRARY_DIRS),-L$(libs) -Wl,-rpath,$(libs)) $(foreach libs,$(LIBRARIES),-l$(libs))
LDLIBS= $(foreach libs,$(LIBRARY_DIRS),-L$(libs) -Wl,-rpath,$(libs)) $(foreach libs,$(LIBRARIES),-l$(libs))
SOURCES=$(wildcard $(SOURCE_DIR)/*.cpp)
# to build executable from all sources in the program directory:
#SOURCES=$(wildcard $(SOURCE_DIR)/*.cpp $(SOURCE_DIR)/*.c)
OBJECTS=$(addprefix $(OBJECT_DIR)/,$(patsubst %.cpp,%.o,$(notdir $(SOURCES))))

C=gcc
CC=g++
CFLAGS=-c -Wall
CCFLAGS=-c -Wall -std=c++11


ifeq ($(COVERAGE),true)
	CFLAGS+= -O0 -g --coverage
	CCFLAGS+= -O0 -g --coverage
	LDFLAGS+= --coverage
endif


.PHONY: all clean run

all: $(SOURCES) $(EXECUTABLE)

clean:
	rm -rf "$(EXECUTABLE)" "$(OBJECT_DIR)"

run: $(SOURCES) $(EXECUTABLE)
	$(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	mkdir -p $(BINARY_DIR)
	$(CC) $(LDFLAGS) $(LDLIBS) $(OBJECTS) -o $@

$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	mkdir -p $(OBJECT_DIR)
	$(CC) $(CCFLAGS) $(INCLUDES) $< -o $@

$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.c
	mkdir -p $(OBJECT_DIR)
	$(C) $(CFLAGS) $(INCLUDES) $< -o $@

