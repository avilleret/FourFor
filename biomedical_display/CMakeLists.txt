project(biomedical_display)

cmake_minimum_required(VERSION 3.10)

include(${OF_ROOT}/openFrameworks.cmake)

ofxaddon(ofxShadertoy)
ofxaddon(ofxFatLines)

set(SOURCES
    src/main.cpp
    src/biomedical_display.cpp
    src/oscilloscope.cpp
)

set(HEADERS
    src/biomedical_display.h
    src/oscilloscope.h
)

add_executable(
    ${PROJECT_NAME}
    MACOSX_BUNDLE
    ${SOURCES}
    ${HEADERS}
    ${OFXADDONS_SOURCES}
)

target_link_libraries(
    ${PROJECT_NAME}
    ${OPENFRAMEWORKS_LIBRARIES}
    ossia
)

ofSetTargetProperties()
