# This file needs to be included before calling `project`.
if (APPLE AND "${CMAKE_GENERATOR}" STREQUAL "Xcode")
  # Generate universal executable for Apple hardware.
  set(CMAKE_OSX_ARCHITECTURES "$(ARCHS_STANDARD)")

  # Support older macOS versions.
  set(CMAKE_OSX_DEPLOYMENT_TARGET 10.15 CACHE STRING "Minimum OS X deployment version")
endif ()
