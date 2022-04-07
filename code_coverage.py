#!/usr/bin python3 
import os   
import sys 

os.chdir(os.getcwd()+'/target/test/c++/irioTests/')

os.system('lcov --directory .  --capture --output-file irio.info')
os.system('genhtml irio.info')
os.system('cp ../../../lib/obj/*.gcda ../../../main/c/lib/')
os.system('cp ../../../lib/obj/*.gcno ../../../main/c/lib/')
os.system('cd ../../../main/c/lib/')
os.system('lcov --directory .  --capture --output-file irio.info')
os.system('genhtml irio.info')
os.system('firefox index.html &')


# /home/jcvillanueva/Desktop/iter/irio-v2/target/test/c++/irioTests

# lcov --directory .  --capture --output-file irio.info
# genhtml irio.info
# cp ../../../lib/obj/*.gcda ../../../main/c/lib/
# cp ../../../lib/obj/*.gcno ../../../main/c/lib/
# cd ../../../main/c/lib/
# lcov --directory .  --capture --output-file irio.info
# genhtml irio.info
# firefox index.html &