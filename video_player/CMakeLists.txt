project(video_player)

cmake_minimum_required(VERSION 3.10)

set(OF_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../of/" CACHE PATH "The root directory of ofnode/of project.")
include(${OF_ROOT}/openFrameworks.cmake)

if(TARGET_ARCH MATCHES arm*)
  ofxaddon(ofxOMXPlayer)

  set(SOURCES
      src/main.cpp
      src/video_player.cpp
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
      pcre
      libilclient.a
      libvcilcs.a
  )

  ofSetTargetProperties()

  install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION .
    LIBRARY DESTINATION lib)
endif()
