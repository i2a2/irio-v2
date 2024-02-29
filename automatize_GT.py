#!/bin/python3
import argparse
import subprocess
import os 
import re
from xml.dom import minidom

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

def runCommand(filterText, RIODevice, RIOSerial, Verbose, Coupling, MaxCounter):
    failed = None
    passed = None
    binary = "driver-irio"
    command = f"\
env -S RIOSerial={RIOSerial} \
env -S RIODevice={RIODevice} \
env -S VerboseInit={1 if Verbose=='true' else 0} \
env -S VerboseTest={1 if Verbose=='true' else 0} \
env -S Coupling={Coupling} \
env -S maxCounter={MaxCounter} \
    ./{binary} --gtest_filter={filterText}" 
    process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, shell=True, bufsize=1)
    while True:
        line = process.stdout.readline().rstrip()

        if not line and process.poll() is not None:
            break

        if line: 
            print(line.decode('utf-8'))

            failmatch = re.findall(r"\[\s+FAILED\s+\] (\d+) test.*", line.decode('utf-8'))
            if len(failmatch) > 0:
                failed = int(failmatch[0])

            passmatch = re.findall(r"\[\s+PASSED\s+\] (\d+) test.*", line.decode('utf-8'))
            if len(passmatch) > 0:
                passed = int(passmatch[0])

    return (passed, (failed if failed is not None else 0) + passed, (failed == 0) or (failed is None))

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
parser.add_argument('-p', '--perf', help='Test performance', action='store_true')
parser.add_argument('--max-counter',help='Max counter for the IMAQ tests',default='65536')
parser.add_argument('TESTPLAN', help='XML file that contains the test plan', nargs='?')
parser.add_argument('RESULTFILE', help='XML file that contains the results of the plan. If a value is not provided, the same file is used', nargs='?')

args = parser.parse_args()

if args.TESTPLAN is None:
    # Parameters
    if args.perf:
        binary = "perf-test"
        path = "/target/test/c++/perfTest/"
    else:
        binary = "driver-irio"
        path = "/target/test/c++/irioTests/"

    # Build command
    if args.list:
        command = f"./{binary} --gtest_list_tests"
    else:
        if args.RIOSerial is not None:
            serial = args.RIOSerial
        else:
            serial = searchSerial(args.RIODevice, args.device_number)

        if args.filter is not None and not args.perf:
            gfilter = " --gtest_filter=" + args.filter
        else:
            gfilter = "" 

        if args.shuffle and not args.perf:
            gshuffle = " --gtest_shuffle"
        else:
            gshuffle = ""

        if args.iterations != '1' and not args.perf:
            giterations = " --gtest-repeat=" + args.iterations
        else:
            giterations = ""

        if args.summary and not args.perf:
            summary = r' | grep -A9999 "Global test environment tear-down"'
        else:
            summary = ""

        command = f"\
    env -S RIOSerial={serial} \
    env -S RIODevice={args.RIODevice} \
    env -S VerboseInit={int(args.verbose_init or args.verbose)} \
    env -S VerboseTest={int(args.verbose_test or args.verbose)} \
    env -S Coupling={args.coupling} \
    env -S maxCounter={args.max_counter} \
    ./{binary}{gfilter}{giterations}{gshuffle}{summary}" 

    if args.verbose or args.verbose_test:
        print(f"Running command: {command}")

    # Runnning tests
    os.chdir(os.getcwd()+path)
    os.system(command)
else:
    if not os.path.exists(args.TESTPLAN):
        print(f"Could not find the file {args.TESTPLAN}")
        exit(-1)
    originaldir = os.getcwd()

    tree = minidom.parse(args.TESTPLAN)
    os.chdir(os.getcwd()+"/target/test/c++/irioTests/")

    for test in tree.getElementsByTagName('test'):
        filterText = test.getElementsByTagName('TestFilter')[0].firstChild.data
        RIODevice  = test.getElementsByTagName('RIODevice')[0].firstChild.data
        RIOSerial  = test.getElementsByTagName('RIOSerial')[0].firstChild.data

        if "UART" in filterText:
            print("\033[91m[TEST] Warning: The UART tests need human interaction \033[00m")

        try: # Optional argument
            verbose = test.getElementsByTagName('verbose')[0].firstChild.data
        except:
            verbose = False 

        try: # Optional argument
            coupling = test.getElementsByTagName('DC')[0].firstChild.data
        except:
            coupling = 'DC' 

        try: # Optional argument
            maxIterations = test.getElementsByTagName('maxIMAQIterations')[0].firstChild.data
        except:
            maxIterations = '65536' 

        if filterText is None or RIODevice is None or RIOSerial is None or verbose is None:
            print("Malformatted test in XML, omitting it")
            continue
        
        result = runCommand(filterText, RIODevice, RIOSerial, verbose, coupling, maxIterations)
        resultElement = tree.createElement("results")
        resultElement.appendChild(tree.createTextNode(f"{result[0]}/{result[1]}"))
        test.appendChild(resultElement)
        summaryElement = tree.createElement("summary")
        summaryElement.appendChild(tree.createTextNode("PASS" if result[2] else "FAIL"))
        test.appendChild(summaryElement)

    os.chdir(originaldir)
    targetFile = args.RESULTFILE if args.RESULTFILE is not None else args.TESTPLAN
    with open(targetFile, 'wt') as fd:
        fd.write("".join([s for s in tree.toprettyxml().strip().splitlines(True) if s.strip("\r\n").strip()]))