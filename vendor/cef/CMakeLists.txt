# Use folders in the resulting project files.
set_property(GLOBAL PROPERTY OS_FOLDERS ON)

# Specify the CEF distribution version.
# https://cef-builds.spotifycdn.com/index.html
set(CEF_VERSION "109.1.16+g454cbc2+chromium-109.0.5414.87")

# Add this project's cmake/ directory to the module path.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Configure CEF build options
set(USE_SANDBOX OFF CACHE BOOL "" FORCE)
set(USE_ATL OFF CACHE BOOL "" FORCE)
set(USE_STATIC_STD_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_TESTS OFF CACHE BOOL "" FORCE)

# Download and extract the CEF binary distribution (executes DownloadCEF.cmake).
include(DownloadCEF.cmake)
include(SupportCEF.cmake)

# determine the current build platform
set_current_cef_build_platform()

DownloadCEF("${CEF_PLATFORM}_minimal" "${CEF_VERSION}" "${CMAKE_CURRENT_BINARY_DIR}/third_party/cef")

# Add the CEF binary distribution's cmake/ directory to the module path.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CEF_ROOT}/cmake")

# Load the CEF configuration (executes FindCEF.cmake).
find_package(CEF REQUIRED)

# Include the libcef_dll_wrapper target (executes libcef_dll/CMakeLists.txt).
add_subdirectory(${CEF_LIBCEF_DLL_WRAPPER_PATH} libcef_dll_wrapper)

target_compile_options(libcef_dll_wrapper PRIVATE "-w")

# Display configuration settings.
PRINT_CEF_CONFIG()
