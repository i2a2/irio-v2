#!/usr/bin python3 
import os    
import argparse

os.chdir(os.getcwd()+"/target/test/c++/irioTests/")

shuffleAndBreak = " --gtest_shuffle"

parser = argparse.ArgumentParser(prog="automatize_GT.py",description='Declare environment variables to automatize GoogleTests execution.')

parser.add_argument('--RIODevice',help='RIO device model. Use $lsrio command to display it',choices=['7961','7965','7966','7975','9159'],required='True')
parser.add_argument('--RIOSerial',help='RIO device serial number. Use $lsrio command to display it',required='True')
parser.add_argument('--allTests',help='Decide if run only funcional tests or funcional tests and error tests. 0 functional ; 1 functional+errors',choices=['1','0'],default='0')
parser.add_argument('--AM',help='Indicate which adapter module is used. It is mandatory that the adapter module match with the RIODevice and RIOSerial used.',choices=['ANALOG','DIG'],default='0')
parser.add_argument('--iterations',help='Select number of iterations of the tests. Pass a negative number to run indefinitely.',default='1')
parser.add_argument('--Coupling',help='Coupling mode for mod5761 test. 1 = DC, 0 = AC',choices=['1','0','DC','AC'],default='0')

# Verbosity
parser.add_argument('-v', '--verbose',help='Print all traces', action='store_true')
parser.add_argument('--verbose-init',help='Print driver startup trace', action='store_true')
parser.add_argument('--verbose-test',help='Print test traces', action='store_true')

args = parser.parse_args()

# Number of iterations
iterations = " --gtest_repeat="+args.iterations

commonFlexRIOTests = "TP_FlexRIO_onlyResources.*:TP_FlexRIO_noModule.*"
if (args.RIODevice=="7966"):
    commonFlexRIOTests += ":TP_FlexRIO_perf.*"

commonCRIOTests = "TP_cRIO_PBP.*:TP_cRIO_DAQ.*"

if (args.RIODevice == "9159"):
    commonTests = commonCRIOTests
else:
    commonTests = commonFlexRIOTests

tests_5761 = "TP_FlexRIO_mod5761.*"
tests_6581 = "TP_FlexRIO_mod6581.*"
errorTests = "TP_errorTests.*"

tests = {"ANALOG": commonTests+":"+tests_5761,
        "DIG": commonTests+":"+tests_6581,
        "0": commonTests}

if (args.verbose_init or args.verbose):
    verbose_init = "1"
else:
    verbose_init = "0"

if (args.verbose_test or args.verbose):
    verbose_test = "1"
else:
    verbose_test = "0"

# if (int(args.allTests)):
#     os.system("env -S RIOSerial="+args.RIOSerial+" env RIODevice="+args.RIODevice+" env Coupling="+args.Coupling+
#               " ./driver-irio --gtest_filter="+tests[args.AM]+":"+errorTests+iterations+shuffleAndBreak)
# else:
#     os.system("env -S RIOSerial="+args.RIOSerial+" env RIODevice="+args.RIODevice+" env Coupling="+args.Coupling+
#               " ./driver-irio --gtest_filter="+tests[args.AM]+iterations+shuffleAndBreak)

# TODO: Remove this, only for test
# TODO: Add verbose variable to previous os.system calls
os.system("env -S RIOSerial="+args.RIOSerial+" env RIODevice="+args.RIODevice+" env VerboseInit="+verbose_init+" env VerboseTest="+verbose_test+" env Coupling="+args.Coupling+ " ./driver-irio")