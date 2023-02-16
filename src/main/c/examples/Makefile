#+======================================================================
# $HeadURL: 
# $Id: 
#
# Project       : IRIO Driver
#
# Description   : IRIO Driver library examples Makefile
#
# Authors       : Mariano Ruiz, Diego Sanz, Sergio Esquembri, Enrique Bernal, Alvaro Bustos
# Authors Affiliation    : Universidad Politécnica de Madrid (UPM) (Technical University of Madrid)
#
# Copyright (C) : 2010-2015 Universidad Politécnica de Madrid (UPM) (Technical University of Madrid)
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


# List of examples to build (source files)
EXAMPLES := cRIO_IO.c cRIO_DAQDMA.c

EXAMPLES += FlexRIO_noModule.c	  
EXAMPLES += FlexRIO_mod5761-7966.c FlexRIO_onlyResources.c
EXAMPLES += FlexRIO_mod5761-7966-perf.c  FlexRIO_mod6581-7965.c FlexRIO_mod1483-7966-Imaq.c FlexRIO_mod1483-7966-uart.c
EXAMPLES += FlexRIO_mod5734-7966.c
EXAMPLES  += Rseries_onlyResources.c
LIBRARIES := m rt irioCore NiFpga niflexrio

COVERAGE=true

ifeq ($(CONFIG_NIRIO_DMA_TO_GPU),1)
	IRIO_GPU = 1
	CFLAGS += -DIRIO_GPU
	EXAMPLES += FlexRIO_mod1483-7966-Imag-GPU.c
	LIBRARIES += iriocuda 
endif


IRIOCORE_DIR := $(realpath ../../../../target/lib)


# Additional include directories


ifndef CODAC_ROOT
#INCLUDE_DIRS += ../support
	CFLAGS += -DCLOSE_VERSION_NIRIO -fPIC -Wall -D_GNU_SOURCE -g
	LDFLAGS += -Wall -Wl,-rpath=$(IRIOCORE_DIR) -Wl,-rpath=/usr/lib/x86_64-linux-gnu
	INCLUDE_DIRS += $(NIRIO)/include ../support/
#	LIBRARY_DIRS =  $(NIRIO)/lib $(IRIOCORE_DIR)
	LIBRARY_DIRS =  /usr/lib /usr/lib/x86_64-linux-gnu $(IRIOCORE_DIR)

else
	CFLAGS += -DOPEN_VERSION_NIRIO -fPIC -Wall -O3 -D_GNU_SOURCE -g
	LDFLAGS += -Wall -Wl,-rpath=$(IRIOCORE_DIR) 
	INCLUDE_DIRS += $(CODAC_ROOT)/include
	LIBRARY_DIRS =  $(CODAC_ROOT)/lib $(IRIOCORE_DIR)
endif

ifeq ($(COVERAGE),true)
	CFLAGS+= -O0 -g --coverage -fPIC -Wall -ggdb -DDEBUG 
	LDFLAGS+= --coverage -Wall -g  -Wl,-rpath=$(IRIOCORE_DIR)
else 
	CFLAGS+= -O3 
endif

ifeq ($(CONFIG_NIRIO_DMA_TO_GPU),1)
	INCLUDE_DIRS += $(CODAC_ROOT)/include/iriocuda
	LDFLAGS_RELEASE += -Wl,-rpath=$(CUDA_DIR)/lib64 
	LDFLAGS_DEBUG += -Wl,-rpath=$(CUDA_DIR)/lib64 
	CUDA_DIR := /usr/local/cuda-6.5
	INCLUDE_DIRS +=$(CUDA_DIR)/include
endif
# Additional library directories


# ======================================================================
# UNIT SPECIFIC COMPILER AND LINKER FLAGS
# ======================================================================

# Optimized release builds (default)
#CFLAGS_RELEASE := -fPIC -Wall -O3
#LDFLAGS_RELEASE := -Wall -Wl,-rpath=$(IRIOCORE_DIR) 

# Debug builds (make debug)
#CFLAGS_DEBUG := -fPIC -Wall -O0 -g -ggdb -DDEBUG 
#LDFLAGS_DEBUG := -Wall -g  -Wl,-rpath=$(IRIOCORE_DIR) 


#ifeq ($(CONFIG_NIRIO_DMA_TO_GPU),1)
#LDFLAGS_RELEASE += -Wl,-rpath=$(CUDA_DIR)/lib64 
#LDFLAGS_DEBUG += -Wl,-rpath=$(CUDA_DIR)/lib64 
#endif
# Uncomment the following lines to use GCov
#CFLAGS_DEBUG += -fprofile-arcs -ftest-coverage
#LDFLAGS_DEBUG += -fprofile-arcs -ftest-coverage -lgcov
########################################


# ======================================================================
# GENERIC BUILD RULES --- DO NOT MODIFY
# ======================================================================

# Sources
sources := $(EXAMPLES)
include_dirs := . ../include $(INCLUDE_DIRS)

# Binaries
alltargets := $(basename $(sources))

# Library dependencies
library_dirs := ../lib $(LIBRARY_DIRS)
libraries := pthread $(LIBRARIES)

# Compiler/linker flags
CFLAGS += $(include_dirs:%=-I"%") 
LDFLAGS += $(library_dirs:%=-L"%") $(libraries:%=-l%) 
#CC := $(if $(findstring .cpp, $(sources)), $(CXX), $(CC))

# Colors
BOLD=\e[1m
NC=\e[0m

# Rules
.PHONY: .depend all release debug clean

all:  release

#release: CFLAGS += $(CFLAGS_RELEASE)
#release: LDFLAGS += $(LDFLAGS_RELEASE)
release: .depend $(alltargets)

#debug: CFLAGS += $(CFLAGS_DEBUG)
#debug: LDFLAGS += $(LDFLAGS_DEBUG)
debug: .depend $(alltargets)

clean:
	@$(RM) -f $(alltargets) .depend *~

%: %.cpp
	@echo -e "$(BOLD)>>> Building '$@' example...$(NC)"
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^

%: %.c
	@echo -e "$(BOLD)>>> Building '$@' example...$(NC)"
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^

# Generate header file dependencies
.depend: $(sources)
	@$(CC) $(CFLAGS) -MM $^ > $@

-include .depend
