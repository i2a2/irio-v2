#+======================================================================
# $HeadURL: 
# $Id: 
#
# Project       : IRIO Driver
#
# Description   : IRIO Driver module Makefile
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

SRCDIR  := src/
COPYDIR := target/
SUBDIRS := $(COPYDIR)main/c $(COPYDIR)test/c++

BOLD=\e[1m
NC=\e[0m

.PHONY: all clean copy build compile coverage debug

all: copy build

clean:
	@echo -e "$(BOLD)Cleaning \"$(COPYDIR)\"...$(NC)"
	rm -rf $(COPYDIR)
	@echo "\"$(COPYDIR)\" cleaned."

copy:
	@echo "Copying $(SRCDIR) to $(COPYDIR)..."
	rsync -a --inplace $(SRCDIR) $(COPYDIR)
	@echo "Copying complete."

build: copy
	@$(foreach dir, $(SUBDIRS), echo -e "$(BOLD)Building $(dir:/=)...$(NC)" && $(MAKE) -C $(dir) $(DEBUG) &&) :

compile: build

coverage:
	$(MAKE) build DEBUG="COVERAGE=true"

debug: coverage