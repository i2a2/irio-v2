#+======================================================================
# $HeadURL$
# $Id$
#
# Project       : CODAC Core System
#
# Description   : Makefile.
#
# Authors       : James Blair (National Instruments)
#                 Mike Hoogendyk (National Instruments)
#
# Copyright (c) : 2013-2017 ITER Organization,
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

TEST = driver-irio

LIBDIR:=$(realpath ../../../../target/lib)

INCLUDES :=-I../../../main/c/include -I../../../main/c/support -I/opt/codac/include -I../include

LIBPATH :=-L../../../lib -Wl,-rpath=$(LIBDIR) -L/opt/codac/lib 

#############################################################################

CXXFLAGS +=$(INCLUDES) --coverage -MMD -MP -g -Wall -std=c++11
LDFLAGS +=$(LIBPATH) -lpthread   -lirioCore -ldl -lrt -lgtest -lboost_regex

#TESTS_OBJS := $(patsubst $(EXAMPLESDIR)/%.cpp, %.o, $(wildcard $(EXAMPLESDIR)/*.cpp))
TESTS_OBJS := $(patsubst %.cpp, %.o, $(wildcard *.cpp))
#TESTS_OBJS := $(EXAMPLESDIR)/ndsirioMain.cpp ndsirioMain.o $(EXAMPLESDIR)/ndsirioFirmware.cpp ndsirioFirmware.o
#TESTS_OBJS := TP_NXD_1000.o TestUtils.o main.o ndsTestFactory.o ndsTestInterface.o


all: $(TEST)

$(TEST): $(TESTS_OBJS) 
	$(CXX) $^ --coverage -o $@ $(LDFLAGS)


clean:
	@$(RM) $(TESTS) *.o *.d
