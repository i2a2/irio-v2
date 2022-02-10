#!/bin/bash
#+======================================================================
# $HeadURL: https://svnpub.iter.org/codac/iter/codac/dev/units/m-irio/trunk/src/test/test.sh $
# $Id: test.sh 127616 2022-01-03 12:25:45Z kimh19 $
#
# Project       : CODAC Core System
#
# Description   : Test script
#
# Author        : janez.golob@cosylab.com
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

SCRIPTDIR=`dirname $0`

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$SCRIPTDIR/../lib

#/sbin/modprobe $SCRIPTDIR/../main/c/driver/pxi6259.ko

#if [ `ls /dev | grep 6259 | wc -l` ]; than
#{
	# Run driver unit tests #
#	./c/driver/runner

	# Run library unit tests #
	./c/lib/runner
#}

#/sbin/rmmod pxi6259

#exit
