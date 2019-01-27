project(biomedical_display)

cmake_minimum_required(VERSION 3.10)

set(OF_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../of/" CACHE PATH "The root directory of ofnode/of project.")
include(${OF_ROOT}/openFrameworks.cmake)

#ofxaddon()

set(SOURCES
    src/main.cpp
    src/biomedical_display.cpp
    src/oscilloscope.cpp
)

add_executable(
    ${PROJECT_NAME}
    MACOSX_BUNDLE
    ${SOURCES}
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
