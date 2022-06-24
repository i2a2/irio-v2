#!/bin/bash

cd target/test/c++/irioTests/
lcov --directory . --capture --output-file irio.info
genhtml irio.info
cd ../../../main/c/lib/
lcov --directory . --capture --output-file irio.info
genhtml irio.info
firefox index.html & 