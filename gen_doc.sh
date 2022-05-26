#!/bin/bash

mvn package
cd target/doc/irio-v2
sed -i 's/IMAGE_PATH             =/IMAGE_PATH             = ./g' doxygen.config
doxygen doxygen.config
cd -
firefox target/doc/irio-v2/html/index.html &

