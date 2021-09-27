cmake_minimum_required(VERSION 3.10)
project(freetype01)

set(SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/sample_freetype.cpp
)

add_executable(
  ${PROJECT_NAME}
  ${SOURCES}
)

target_include_directories(
  ${PROJECT_NAME} PRIVATE
  ${LIBRARY_INCLUDE_DIR}/freetype2
)

target_link_directories(
  ${PROJECT_NAME} PRIVATE
  ${LIBRARY_LIB_DIR}
)

target_link_libraries(
  ${PROJECT_NAME} PRIVATE
  ${LINK_LIBRARIES}
)
