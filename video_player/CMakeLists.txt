project(video_player)

set(CMAKE_CXX_STANDARD 17)

cmake_minimum_required(VERSION 3.10)

include(${OF_ROOT}/openFrameworks.cmake)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../common/src)

if(TARGET_ARCH MATCHES arm*)
  ofxaddon(ofxOMXPlayer)
  add_definitions(-DOMXPLAYER)
endif()

  ofxaddon(ofxFatLines)

  set(SOURCES
    src/main.cpp
    src/video_player.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../common/src/OssiaText.cpp
  )

  set(HEADERS
    src/video_player.h
    src/SafeImage.h
    src/SafePlayer.h
    ${CMAKE_CURRENT_LIST_DIR}/../common/src/OssiaText.h
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
if(TARGET_ARCH MATCHES arm*)
  target_link_libraries(
      ${PROJECT_NAME}
      pcre
      libilclient.a
      libvcilcs.a
      )
endif()

  ofSetTargetProperties()

  install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION .
    LIBRARY DESTINATION lib)
