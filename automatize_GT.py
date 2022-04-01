#!/usr/bin python3 
import os   
import sys 

# It is mandatory to compile the project before execute this script
os.chdir(os.getcwd()+'/target/test/c++/irioTests/')

# Number of iterations
iterations = "1"

# Do not make sense to add --gtest_shuffle parameter because the wey is coded
# this script 
shuffleAndBreak = ' --gtest_break_on_failure'

# Environment variables
if (len(sys.argv) == 1): # Do not pass environment variables, only run the script
    allTests = True      # All tests are going to be executed with his needed environment variables
else:
    allTests = False     # Only run the right tests for each set of environment variables
    RIOSerial = sys.argv[1]
    RIODevice = sys.argv[2]
    if (RIODevice == "7966" and RIOSerial == "0x0177A2AD"):   
        Coupling = sys.argv[3]
        
    if (RIODevice=="9159" and RIOSerial=="0x01C10FAC"):
        print("Añadir parametros de test cRIO")

# Tests for all FlexRIO devices 
################################################
#                TODO
# Cambiar esta lista generica a una lista comun y a una lista para cada tarjeta a testear
# Una vez funcione esa separacion en distintas listas pasarlo a diccionarios para asignacion key-value
################################################


# v1.0
#

tests = [
         [ 
          ["TP_FlexRIO_onlyResources",".CPUDAQ"],
          ["TP_FlexRIO_onlyResources",".CPUIMAQ"],
          ["TP_FlexRIO_onlyResources",".onlyResources"],
          ["TP_FlexRIO_onlyResources",".wrongFPGAVersion"],
          ["TP_FlexRIO_onlyResources",".wrongFilePath"],
          ["TP_FlexRIO_onlyResources",".wrongBitfileName"],
          ["TP_FlexRIO_onlyResources",".wrongRIOSerial"],
          ["TP_FlexRIO_onlyResources",".wrongRIODevice"],
          ["TP_FlexRIO_onlyResources",".differentModel_Serial"],
          ["TP_FlexRIO_onlyResources",".wrongBitfileResources"]
        ],            
# Tests for all FlexRIO devices         
        [ 
          ["TP_FlexRIO_noModule",".functional"],
          ["TP_FlexRIO_noModule",".setFPGATwice"],
          ["TP_FlexRIO_noModule",".setFPGAInitErrorSessionClose"],
          ["TP_FlexRIO_noModule",".setFPGAInitErrorSessionOpen"],
          ["TP_FlexRIO_noModule",".manageNonImplementedPorts"]
        ],
# Tests for PXIe-7966R and NI5761 module, RIODevice=7966, RIOSerial=0x0177A2AD        
        [ 
          ["TP_FlexRIO_mod5761",".functional"],
          ["TP_FlexRIO_mod5761",".failInitDriver"]
        ],
# Tests for PXIe-7966R and NI5761 module focused on performance, RIODevice=7966, RIOSerial=0x0177A2AD        
        # [ 
        #   ["TP_FlexRIO_mod5761_perf",".functional"],
        # ],
# Tests for PXIe-7966R and NI6581 module, RIODevice=7965, RIOSerial=0x01666C59        
        [ 
          ["TP_FlexRIO_mod6581",".functional"],
          ["TP_FlexRIO_mod6581",".failInitDriver"]
        ],         
# Tests for PXIe-7966R and NI1483 module, RIODevice=7966, RIOSerial=0x01A34CC7        
        [ 
          #["TP_FlexRIO_mod1483",".functionalUART"], This test is designed but can't be tested automatically
          ["TP_FlexRIO_mod1483",".failInitDriverUART"], 
          ["TP_FlexRIO_mod1483",".functionalIMAQ"],
          ["TP_FlexRIO_mod1483",".failInitDriverIMAQ"]
        ],    
# Tests for NI9158 chassis with cRIO modules 
        # [ 
        #   ["TP_cRIO_DAQ",".functional"], 
        # ],
         # [ 
        #   ["TP_cRIO_PBP",".functional"],    
        # ]
         ]
f = open('ErrorFile', 'w')
for test in tests: # tests = [ onlyResources, noModule, mod5761, mod6581, mod1483 ]
    for test_ in test: # test = [ CPUDAQ, CPUIMAQ, failInitDriver, ... ]
        if (allTests):
            RIODevice = "7966"; RIOSerial = "0x0177A2AD"; Coupling = "0"
            if (test_[0] != "TP_FlexRIO_mod6581" and test_[0] !="TP_FlexRIO_mod1483"):
                if os.system('env -S RIOSerial='+RIOSerial+' env RIODevice='+RIODevice+' env Coupling='+Coupling+' ./driver-irio --gtest_filter='+test_[0]+test_[1]+' --gtest_repeat='+iterations+shuffleAndBreak) != 0:
                    f.write(test_[0]+test_[1]+'\n')
                RIODevice="7965"; RIOSerial="0x01666C59"
            if (test_[0] != "TP_FlexRIO_mod5761" and test_[0] != "TP_FlexRIO_mod5761_perf" and test_[0] !="TP_FlexRIO_mod1483"):
                if os.system('env -S RIOSerial='+RIOSerial+' env RIODevice='+RIODevice+' ./driver-irio --gtest_filter='+test_[0]+test_[1]+' --gtest_repeat='+iterations+shuffleAndBreak) != 0:
                    f.write(test_[0]+test_[1]+'\n')

            RIODevice="7966"; RIOSerial="0x01A34CC7"
            if (test_[0] != "TP_FlexRIO_mod5761" and test_[0] != "TP_FlexRIO_mod5761_perf" and test_[0] !="TP_FlexRIO_mod6581"):
                if os.system('env -S RIOSerial='+RIOSerial+' env RIODevice='+RIODevice+' ./driver-irio --gtest_filter='+test_[0]+test_[1]+' --gtest_repeat='+iterations+shuffleAndBreak) != 0:
                    f.write(test_[0]+test_[1]+'\n')

        else: 
            if (RIODevice=="7966" and RIOSerial=="0x0177A2AD" and test_[0] != "TP_FlexRIO_mod6581" and test_[0] !="TP_FlexRIO_mod1483"):
                if os.system('env -S RIOSerial='+RIOSerial+' env RIODevice='+RIODevice+' env Coupling='+Coupling+' ./driver-irio --gtest_filter='+test_[0]+test_[1]+' --gtest_repeat='+iterations+shuffleAndBreak) != 0:
                    f.write(test_[0]+test_[1]+'\n')
            if (RIODevice=="7965" and RIOSerial=="0x01666C59" and test_[0] != "TP_FlexRIO_mod5761" and test_[0] != "TP_FlexRIO_mod5761_perf" and test_[0] !="TP_FlexRIO_mod1483"):
                if os.system('env -S RIOSerial='+RIOSerial+' env RIODevice='+RIODevice+' ./driver-irio --gtest_filter='+test_[0]+test_[1]+' --gtest_repeat='+iterations+shuffleAndBreak) != 0:
                    f.write(test_[0]+test_[1]+'\n')

            if (RIODevice=="7966" and RIOSerial=="0x01A34CC7" and test_[0] != "TP_FlexRIO_mod5761" and test_[0] != "TP_FlexRIO_mod5761_perf" and test_[0] !="TP_FlexRIO_mod6581"):
                if os.system('env -S RIOSerial='+RIOSerial+' env RIODevice='+RIODevice+' ./driver-irio --gtest_filter='+test_[0]+test_[1]+' --gtest_repeat='+iterations+shuffleAndBreak) != 0:
                    f.write(test_[0]+test_[1]+'\n')
#
#         # Añadir performance cuando este listo el test 
#
#         # Añadir compactRIO cuando esten listos los tests    
#
f.close()

####################################################
# v1.1   No funciona aun 

# functionalTests = [
#         "TP_FlexRIO_onlyResources.CPUDAQ",
#         "TP_FlexRIO_onlyResources.CPUIMAQ",
#         "TP_FlexRIO_onlyResources.onlyResources",
#         "TP_FlexRIO_noModule.functional",
#         "TP_FlexRIO_mod5761.functional",
#         "TP_FlexRIO_mod6581.functional",
#         "TP_FlexRIO_mod1483.functionalIMAQ"
#         ]
#
# errorTests = [
#               "TP_FlexRIO_onlyResources.*",
#               "TP_FlexRIO_noModule.*",
#               "TP_FlexRIO_mod5761.*",
#               "TP_FlexRIO_mod5761_perf.*",
#               "TP_FlexRIO_mod6581.*",
#               "TP_FlexRIO_mod1483.*",  
#               ]
#
# f = open('ErrorFile', 'w')
# if (allTests):
#     for test in errorTests:
#         RIODevice = "7966"; RIOSerial = "0x0177A2AD"; Coupling = "0"
#         if (test != "TP_FlexRIO_mod6581" and test !="TP_FlexRIO_mod1483"):
#             if os.system('env -S RIOSerial='+RIOSerial+' env RIODevice='+RIODevice+' env Coupling='+Coupling+' ./driver-irio --gtest_filter='+test+' --gtest_repeat='+iterations+shuffleAndBreak) != 0:
#                 f.write(test+'\n')
#
#         RIODevice="7965"; RIOSerial="0x01666C59"
#         if (test != "TP_FlexRIO_mod5761" and test != "TP_FlexRIO_mod5761_perf" and test !="TP_FlexRIO_mod1483"):
#             if os.system('env -S RIOSerial='+RIOSerial+' env RIODevice='+RIODevice+' ./driver-irio --gtest_filter='+test+test+' --gtest_repeat='+iterations+shuffleAndBreak) != 0:
#                 f.write(test+'\n')
#
#         RIODevice="7966"; RIOSerial="0x01A34CC7"
#         if (test != "TP_FlexRIO_mod5761" and test != "TP_FlexRIO_mod5761_perf" and test !="TP_FlexRIO_mod6581"):
#             if os.system('env -S RIOSerial='+RIOSerial+' env RIODevice='+RIODevice+' ./driver-irio --gtest_filter='+test+' --gtest_repeat='+iterations+shuffleAndBreak) != 0:
#                 f.write(test+'\n')        
#
# else:
#     for test in functionalTests:
#         if (RIODevice=="7966" and RIOSerial=="0x0177A2AD" and test != "TP_FlexRIO_mod6581" and test !="TP_FlexRIO_mod1483"):
#             if os.system('env -S RIOSerial='+RIOSerial+' env RIODevice='+RIODevice+' env Coupling='+Coupling+' ./driver-irio --gtest_filter='+test+' --gtest_repeat='+iterations+shuffleAndBreak) != 0:
#                 f.write(test+'\n')
#
#         if (RIODevice=="7965" and RIOSerial=="0x01666C59" and test != "TP_FlexRIO_mod5761" and test != "TP_FlexRIO_mod5761_perf" and test !="TP_FlexRIO_mod1483"):
#             if os.system('env -S RIOSerial='+RIOSerial+' env RIODevice='+RIODevice+' ./driver-irio --gtest_filter='+test+' --gtest_repeat='+iterations+shuffleAndBreak) != 0:
#                 f.write(test+'\n')
#
#         if (RIODevice=="7966" and RIOSerial=="0x01A34CC7" and test != "TP_FlexRIO_mod5761" and test != "TP_FlexRIO_mod5761_perf" and test !="TP_FlexRIO_mod6581"):
#             if os.system('env -S RIOSerial='+RIOSerial+' env RIODevice='+RIODevice+' ./driver-irio --gtest_filter='+test+' --gtest_repeat='+iterations+shuffleAndBreak) != 0:
#                 f.write(test+'\n')
#
#
# f.close()

os.system('mv ErrorFile ../../../../')

print("See file ErrorFile on that directory to check if a test has failed")




