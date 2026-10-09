# CMake generated Testfile for 
# Source directory: /Volumes/data1/AI_Gerelateerd/github/muziek_gerelateerd/C64-SID-Studio
# Build directory: /Volumes/data1/AI_Gerelateerd/github/muziek_gerelateerd/C64-SID-Studio/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test("sid64_arpeggiator" "/Volumes/data1/AI_Gerelateerd/github/muziek_gerelateerd/C64-SID-Studio/build/sid64_arp_tests")
set_tests_properties("sid64_arpeggiator" PROPERTIES  _BACKTRACE_TRIPLES "/Volumes/data1/AI_Gerelateerd/github/muziek_gerelateerd/C64-SID-Studio/CMakeLists.txt;17;add_test;/Volumes/data1/AI_Gerelateerd/github/muziek_gerelateerd/C64-SID-Studio/CMakeLists.txt;0;")
add_test("sid64_sequence" "/Volumes/data1/AI_Gerelateerd/github/muziek_gerelateerd/C64-SID-Studio/build/sid64_sequence_tests")
set_tests_properties("sid64_sequence" PROPERTIES  _BACKTRACE_TRIPLES "/Volumes/data1/AI_Gerelateerd/github/muziek_gerelateerd/C64-SID-Studio/CMakeLists.txt;18;add_test;/Volumes/data1/AI_Gerelateerd/github/muziek_gerelateerd/C64-SID-Studio/CMakeLists.txt;0;")
add_test("sid64_engine" "/Volumes/data1/AI_Gerelateerd/github/muziek_gerelateerd/C64-SID-Studio/build/sid64_tests")
set_tests_properties("sid64_engine" PROPERTIES  _BACKTRACE_TRIPLES "/Volumes/data1/AI_Gerelateerd/github/muziek_gerelateerd/C64-SID-Studio/CMakeLists.txt;19;add_test;/Volumes/data1/AI_Gerelateerd/github/muziek_gerelateerd/C64-SID-Studio/CMakeLists.txt;0;")
add_test("sid64_character" "/Volumes/data1/AI_Gerelateerd/github/muziek_gerelateerd/C64-SID-Studio/build/sid64_character_tests")
set_tests_properties("sid64_character" PROPERTIES  _BACKTRACE_TRIPLES "/Volumes/data1/AI_Gerelateerd/github/muziek_gerelateerd/C64-SID-Studio/CMakeLists.txt;20;add_test;/Volumes/data1/AI_Gerelateerd/github/muziek_gerelateerd/C64-SID-Studio/CMakeLists.txt;0;")
subdirs("_deps/juce-build")
