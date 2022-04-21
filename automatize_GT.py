#!/usr/bin python3 
import os   
import sys 

# It is mandatory to compile the project before execute this script
#os.system("mvn clean compile -D coverage")

os.chdir(os.getcwd()+"/target/test/c++/irioTests/")

# Number of iterations
iterations = " --gtest_repeat=1"

shuffleAndBreak = " --gtest_break_on_failure --gtest_shuffle"

allTests  = sys.argv[1]
RIOSerial = sys.argv[2]
RIODevice = sys.argv[3]

if (RIODevice == "7966" and RIOSerial == "0x0177A2AD"):   
    Coupling = sys.argv[4]
    
f = open('ErrorFile', 'w')

functionalTests_5761 = ["TP_FlexRIO_onlyResources.CPUDAQ:TP_FlexRIO_onlyResources.CPUIMAQ:TP_FlexRIO_onlyResources.onlyResources:"+
                        "TP_FlexRIO_noModule.functional:TP_FlexRIO_mod5761.functional"]
                        # :TP_FlexRIO_mod5761_perf.functional"]

functionalTests_6581 = ["TP_FlexRIO_onlyResources.CPUDAQ:TP_FlexRIO_onlyResources.CPUIMAQ:TP_FlexRIO_onlyResources.onlyResources:"+
                        "TP_FlexRIO_noModule.functional:TP_FlexRIO_mod6581.functional"]

tests_5761 = ["TP_FlexRIO_onlyResources.*:TP_FlexRIO_noModule.*:TP_FlexRIO_mod5761.*"]
              # TP_FlexRIO_mod5761_perf.*"]
              
tests_6581 = ["TP_FlexRIO_onlyResources.*:TP_FlexRIO_noModule.*:TP_FlexRIO_mod6581.*"]

tests_1483 = ["TP_FlexRIO_onlyResources.*:TP_FlexRIO_noModule.*"]

# PXIe-7966R device + NI5761 module
if(RIODevice=="7966" and RIOSerial=="0x0177A2AD" and int(allTests)):
    for test in tests_5761:
        if os.system("env -S RIOSerial="+RIOSerial+" env RIODevice="+RIODevice+" env Coupling="+Coupling+
                     " ./driver-irio --gtest_filter="+test+iterations+shuffleAndBreak):
            f.write(test+"\n")
elif (RIODevice=="7966" and RIOSerial=="0x0177A2AD" and not(int(allTests))):
     for test in functionalTests_5761:
        if os.system("env -S RIOSerial="+RIOSerial+" env RIODevice="+RIODevice+" env Coupling="+Coupling+
                     " ./driver-irio --gtest_filter="+test+iterations+shuffleAndBreak):
            f.write(test+"\n")
            

# PXIe-7965R device + NI6581 module            
if(RIODevice=="7965" and RIOSerial=="0x01666C59" and int(allTests)):
    for test in tests_6581:
        if os.system("env -S RIOSerial="+RIOSerial+" env RIODevice="+RIODevice+
                     " ./driver-irio --gtest_filter="+test+iterations+shuffleAndBreak):
            f.write(test+"\n")
elif (RIODevice=="7965" and RIOSerial=="0x01666C59" and not(int(allTests))):
     for test in functionalTests_6581:
        if os.system("env -S RIOSerial="+RIOSerial+" env RIODevice="+RIODevice+
                     " ./driver-irio --gtest_filter="+test+iterations+shuffleAndBreak):
            f.write(test+"\n")


# PXIe-7966R device + NI1483 module have to be executed manually 


f.close()

os.system('mv ErrorFile ../../../../')
print("See file ErrorFile on that directory to check if a test has failed")

