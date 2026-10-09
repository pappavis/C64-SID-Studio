# CMake generated Testfile for 
# Source directory: /Users/michiele/Downloads/SID-Studio-64-Ronde3C1-Fix-v0.3.5
# Build directory: /Users/michiele/Downloads/SID-Studio-64-Ronde3C1-Fix-v0.3.5/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test("sid64_arpeggiator" "/Users/michiele/Downloads/SID-Studio-64-Ronde3C1-Fix-v0.3.5/build/sid64_arp_tests")
set_tests_properties("sid64_arpeggiator" PROPERTIES  _BACKTRACE_TRIPLES "/Users/michiele/Downloads/SID-Studio-64-Ronde3C1-Fix-v0.3.5/CMakeLists.txt;17;add_test;/Users/michiele/Downloads/SID-Studio-64-Ronde3C1-Fix-v0.3.5/CMakeLists.txt;0;")
add_test("sid64_sequence" "/Users/michiele/Downloads/SID-Studio-64-Ronde3C1-Fix-v0.3.5/build/sid64_sequence_tests")
set_tests_properties("sid64_sequence" PROPERTIES  _BACKTRACE_TRIPLES "/Users/michiele/Downloads/SID-Studio-64-Ronde3C1-Fix-v0.3.5/CMakeLists.txt;18;add_test;/Users/michiele/Downloads/SID-Studio-64-Ronde3C1-Fix-v0.3.5/CMakeLists.txt;0;")
add_test("sid64_engine" "/Users/michiele/Downloads/SID-Studio-64-Ronde3C1-Fix-v0.3.5/build/sid64_tests")
set_tests_properties("sid64_engine" PROPERTIES  _BACKTRACE_TRIPLES "/Users/michiele/Downloads/SID-Studio-64-Ronde3C1-Fix-v0.3.5/CMakeLists.txt;19;add_test;/Users/michiele/Downloads/SID-Studio-64-Ronde3C1-Fix-v0.3.5/CMakeLists.txt;0;")
add_test("sid64_character" "/Users/michiele/Downloads/SID-Studio-64-Ronde3C1-Fix-v0.3.5/build/sid64_character_tests")
set_tests_properties("sid64_character" PROPERTIES  _BACKTRACE_TRIPLES "/Users/michiele/Downloads/SID-Studio-64-Ronde3C1-Fix-v0.3.5/CMakeLists.txt;20;add_test;/Users/michiele/Downloads/SID-Studio-64-Ronde3C1-Fix-v0.3.5/CMakeLists.txt;0;")
subdirs("_deps/juce-build")
