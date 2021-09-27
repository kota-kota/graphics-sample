cmake_minimum_required(VERSION 3.10)
project(graphics-sample)

include(cmake/setup.cmake)

if(MSVC)
  # Visual C (Windows)
  set(LINK_LIBRARIES
    debug Debug/freetyped optimized Release/freetype
    debug Debug/libpng16_staticd optimized Release/libpng16_static
    debug Debug/zlibstaticd optimized Release/zlibstatic
  )

elseif(ANDROID)
  # Android

else()
  # Linux

endif()

add_subdirectory(freetype01)
