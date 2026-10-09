# Vliegbasis71 SID Studio 64 | v0.3.5 | ChatID C5A9E2D7
cmake_minimum_required(VERSION 3.22)
project(SIDStudio64 VERSION 0.3.5 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_library(sid64 STATIC src/SidEngine.cpp)
target_include_directories(sid64 PUBLIC src)
add_executable(sid64_tests tests/test_engine.cpp)
target_link_libraries(sid64_tests PRIVATE sid64)
add_executable(sid64_character_tests tests/test_sid_character.cpp)
target_link_libraries(sid64_character_tests PRIVATE sid64)
add_executable(sid64_arp_tests tests/test_arpeggiator.cpp src/Arpeggiator.cpp)
target_include_directories(sid64_arp_tests PRIVATE src)
add_executable(sid64_sequence_tests tests/test_sequence.cpp src/StepSequencer.cpp src/Arpeggiator.cpp)
target_include_directories(sid64_sequence_tests PRIVATE src)
enable_testing()
add_test(NAME sid64_arpeggiator COMMAND sid64_arp_tests)
add_test(NAME sid64_sequence COMMAND sid64_sequence_tests)
add_test(NAME sid64_engine COMMAND sid64_tests)
add_test(NAME sid64_character COMMAND sid64_character_tests)
option(SID64_BUILD_PLUGIN "Build Audio Unit plugin (requires JUCE)" ${APPLE})
if(SID64_BUILD_PLUGIN)
  if(NOT APPLE)
    message(FATAL_ERROR "Audio Unit builds require macOS")
  endif()
  include(FetchContent)
  FetchContent_Declare(JUCE GIT_REPOSITORY https://github.com/juce-framework/JUCE.git GIT_TAG 8.0.10 GIT_SHALLOW TRUE)
  FetchContent_MakeAvailable(JUCE)
  juce_add_plugin(SIDStudio64
    COMPANY_NAME "Vliegbasis71"
    BUNDLE_ID "nl.vliegbasis71.sidstudio64"
    PLUGIN_MANUFACTURER_CODE Vl71
    PLUGIN_CODE Si64
    FORMATS AU Standalone
    PRODUCT_NAME "SID Studio 64"
    PLUGIN_VERSION "0.3.5"
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
  )
  target_sources(SIDStudio64 PRIVATE src/PluginProcessor.cpp src/PluginEditor.cpp src/StepSequencer.cpp src/Arpeggiator.cpp)
  target_link_libraries(SIDStudio64 PRIVATE sid64 juce::juce_audio_utils juce::juce_audio_processors juce::juce_gui_extra juce::juce_recommended_config_flags juce::juce_recommended_warning_flags)
  target_compile_definitions(SIDStudio64 PUBLIC JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0 JUCE_VST3_CAN_REPLACE_VST2=0)
endif()
