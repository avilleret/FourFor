project(biomedical_display)

cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

include(${OF_ROOT}/openFrameworks.cmake)

ofxaddon(ofxShadertoy)
ofxaddon(ofxFatLines)

include_directories(${CMAKE_CURRENT_LIST_DIR}/../common/src)

set(SOURCES
    src/main.cpp
    src/biomedical_display.cpp
    src/oscilloscope.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../common/src/OssiaText.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../common/src/OssiaShader.cpp
)

set(HEADERS
    src/biomedical_display.h
    src/oscilloscope.h
    ${CMAKE_CURRENT_LIST_DIR}/../common/src/OssiaText.h
    ${CMAKE_CURRENT_LIST_DIR}/../common/src/OssiaShader.h
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
