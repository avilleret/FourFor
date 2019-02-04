project(biomedical_display)

cmake_minimum_required(VERSION 3.10)

set(OF_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../of/" CACHE PATH "The root directory of ofnode/of project.")
include(${OF_ROOT}/openFrameworks.cmake)

ofxaddon(ofxShadertoy)

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

install(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION .
  LIBRARY DESTINATION lib)

install(FILES
  ${OF_LIB_DIR}/libopenFrameworks.so
  DESTINATION lib)

if(NOT TARGET_ARCH MATCHES arm*)
  install(FILES
    ${OF_LIB_DIR}/libglfw.so
    ${OF_LIB_DIR}/libglfw.so.3
    ${OF_LIB_DIR}/libglfw.so.3.1
    DESTINATION lib)
endif()
