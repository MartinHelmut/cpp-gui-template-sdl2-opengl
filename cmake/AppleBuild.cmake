# This file needs to be included before calling `project`.
if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  # Generate universal executable for Apple hardware.
  set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "")

  # Support older macOS versions.
  set(CMAKE_OSX_DEPLOYMENT_TARGET 10.15 CACHE STRING "Minimum OS X deployment version")
  set(CMAKE_OSX_SYSROOT macosx10.15)
endif ()
