#!/usr/bin python3 
import os   
import sys 
import argparse

# It is mandatory to compile the project before execute this script
#os.system("mvn clean compile -D coverage")

os.chdir(os.getcwd()+"/target/test/c++/irioTests/")

# Number of iterations
iterations = " --gtest_repeat=1"

shuffleAndBreak = " --gtest_break_on_failure --gtest_shuffle"

parser = argparse.ArgumentParser(prog="automatize_GT.py",description='Declare environment variables to automatize GoogleTests execution.')

parser.add_argument('--RIODevice',help='RIO device model. Use $lsrio command to display it',choices=['7961','7965','7966','7975','9159'],required='True')
parser.add_argument('--RIOSerial',help='RIO device serial number. Use $lsrio command to display it',required='True')
parser.add_argument('--allTests',help='Decide if run only funcional tests or funcional tests and error tests. 0 functional ; 1 functional+errors',choices=['1','0'],default='0')
parser.add_argument('--AM',help='Indicate which adapter module is used. It is mandatory that the adapter module match with the RIODevice and RIOSerial used.',choices=['ANALOG','DIG'],default='0')
parser.add_argument('--Coupling',help='Coupling mode for mod5761 test. 1 = DC, 0 = AC',choices=['1','0','DC','AC'],default='0')

args = parser.parse_args()

commonFlexRIOTests = "TP_FlexRIO_onlyResources.*:TP_FlexRIO_noModule.*"
commonCRIOTests = "TP_cRIO_PBP.*:TP_cRIO_DAQ.*"

if (args.RIODevice == "9159"):
    commonTests = commonCRIOTests
else:
    commonTests = commonFlexRIOTests

tests_5761 = "TP_FlexRIO_mod5761.*"
tests_6581 = "TP_FlexRIO_mod6581.*"
errorTests = "TP_errorTests.*"

tests = {"ANALOG": commonTests+":"+"tests_5761"+":"+"TP_FlexRIO_perf.*",
        "DIG": commonTests+":"+"tests_6581",
        "0": commonTests}


f = open('ErrorFile', 'w')
if (int(args.allTests)):
    os.system("env -S RIOSerial="+args.RIOSerial+" env RIODevice="+args.RIODevice+" env Coupling="+args.Coupling+
              " ./driver-irio --gtest_filter="+tests[args.AM]+":"+errorTests+iterations+shuffleAndBreak)
else:
    os.system("env -S RIOSerial="+args.RIOSerial+" env RIODevice="+args.RIODevice+" env Coupling="+args.Coupling+
              " ./driver-irio --gtest_filter="+tests[args.AM]+iterations+shuffleAndBreak)

f.close()

os.system('mv ErrorFile ../../../../')
print("See file ErrorFile on that directory to check if a test has failed")

