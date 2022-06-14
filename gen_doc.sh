#!/bin/bash

mvn package
cd target/doc/irio
doxygen doxygen.config
cd -
firefox target/doc/irio/html/index.html &
