project(video_player)

cmake_minimum_required(VERSION 3.10)

include(${OF_ROOT}/openFrameworks.cmake)

if(TARGET_ARCH MATCHES arm*)
  ofxaddon(ofxOMXPlayer)

  set(SOURCES
      src/main.cpp
      src/video_player.cpp
  )

  set(HEADERS
    src/video_player.h
    src/SafeImage.h
    src/SafePlayer.h
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
      pcre
      libilclient.a
      libvcilcs.a
  )

  ofSetTargetProperties()

  install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION .
    LIBRARY DESTINATION lib)
endif()
