#!/bin/python3
import argparse
import subprocess
import os 
import re

if __name__ != "__main__":
    exit(1)

def searchSerial(device, order):
    if device not in ['7961','7965','7966','7975','9159']:
        raise Exception("Invalid device type")
    
    results = re.findall(f'--Model Name:[^\\n]*{device}[^\\n]*\\n\\s*--Serial Number:\\s*(\\S+)\\n', subprocess.check_output(['lsrio']).decode())

    if len(results) == 0:
        raise Exception("No devices found")
    else:
        return results[max(min(int(order) - 1, len(results) - 1), 0)]
    

# Parameters
binary = "driver-irio"

# Parse arguments
parser = argparse.ArgumentParser(
    prog="automatize_GT.py",
    description='Declare environment variables to automatize GoogleTests execution.',
)

parser.add_argument('--RIODevice',help='RIO device model. Use lsrio command to display it. If no serial number is provided, the first device is selected',choices=['7961','7965','7966','7975','9159'], default='7966')
parser.add_argument('--RIOSerial',help='RIO device serial number. Use $lsrio command to display it')
# parser.add_argument('--AM',help='Indicate which adapter module is used. It is mandatory that the adapter module match with the RIODevice and RIOSerial used.',choices=['ANALOG','DIG'],default='0')
parser.add_argument('-d', '--device-number',help='If no RIOSerial is provided and there are multiple devices, select which one to use. Bounded between 0 and the number of devices',default='1')
parser.add_argument('-i', '--iterations',help='Select number of iterations of the tests. Pass a negative number to run indefinitely.',default='1')
parser.add_argument('-s', '--shuffle',help='Shuffle the test execution', action='store_true')
parser.add_argument('-c', '--coupling',help='Coupling mode for mod5761 test. 1 = DC, 0 = AC',choices=['1','0','DC','AC'],default='0')
parser.add_argument('-v', '--verbose',help='Print all traces', action='store_true')
parser.add_argument('--verbose-init',help='Print driver startup trace', action='store_true')
parser.add_argument('--verbose-test',help='Print test traces', action='store_true')
parser.add_argument('-f', '--filter', help='Filter the text execution', )
parser.add_argument('-l', '--list', help='List all the tests', action='store_true')
parser.add_argument('-S', '--summary',help='Summarize the execution', action='store_true')

args = parser.parse_args()

# Build command
if args.list:
    command = f"./{binary} --gtest_list_tests"
else:
    if args.RIOSerial is not None:
        serial = args.RIOSerial
    else:
        serial = searchSerial(args.RIODevice, args.device_number)

    if args.filter is not None:
        gfilter = " --gtest_filter=" + args.filter
    else:
        gfilter = "" 

    if args.shuffle:
        gshuffle = " --gtest_shuffle"
    else:
        gshuffle = ""

    if args.iterations != '1':
        giterations = " --gtest-repeat=" + args.iterations
    else:
        giterations = ""

    if args.summary:
        summary = r' | grep -A9999 "Global test environment tear-down"'
    else:
        summary = ""

    command = f"\
env -S RIOSerial={serial} \
env -S RIODevice={args.RIODevice} \
env -S VerboseInit={int(args.verbose_init or args.verbose)} \
env -S VerboseTest={int(args.verbose_test or args.verbose)} \
env -S Coupling={args.coupling} \
./{binary}{gfilter}{giterations}{gshuffle}{summary}" 

# Runnning tests
os.chdir(os.getcwd()+"/target/test/c++/irioTests/")
os.system(command)