function(set_current_cef_build_platform)
  if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
    set(CEF_PLATFORM "macosx64" PARENT_SCOPE)
  elseif ("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
    if (CMAKE_SIZEOF_VOID_P MATCHES 8)
      set(CEF_PLATFORM "linux64" PARENT_SCOPE)
    else ()
      set(CEF_PLATFORM "linux32" PARENT_SCOPE)
    endif ()
  elseif ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    if (CMAKE_SIZEOF_VOID_P MATCHES 8)
      set(CEF_PLATFORM "windows64" PARENT_SCOPE)
    else ()
      set(CEF_PLATFORM "windows32" PARENT_SCOPE)
    endif ()
  endif ()
endfunction()

macro(set_cef_target_out_dir)
  if (${CMAKE_GENERATOR} STREQUAL "Ninja" OR
    ${CMAKE_GENERATOR} STREQUAL "Unix Makefiles")
    # By default Ninja and Make builds don't create a subdirectory named after
    # the configuration.
    # set(CEF_TARGET_OUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
    set(CEF_TARGET_OUT_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}")

    # Output binaries (executables, libraries) to the correct directory.
    # set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CEF_TARGET_OUT_DIR})
    # set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CEF_TARGET_OUT_DIR})
  else ()
    # set(CEF_TARGET_OUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>")
    set(CEF_TARGET_OUT_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>")
  endif ()
endmacro()

macro(add_windows_cef_manifest target_dir manifest_path target extension)
  add_custom_command(
    TARGET ${target}
    POST_BUILD
    COMMAND "mt.exe" -nologo
    -manifest \"${manifest_path}/${target}.${extension}.manifest\" \"${manifest_path}/compatibility.manifest\"
    -outputresource: "${target_dir}/${target}.${extension}" \;\#1
    COMMENT "Adding manifest..."
  )
endmacro()


# Add a logical target that can be used to link the specified libraries into an
# executable target.
macro(add_cef_logical_target target debug_lib release_lib)
  add_library(${target} ${CEF_LIBTYPE} IMPORTED GLOBAL)
  set_target_properties(${target} PROPERTIES
    IMPORTED_LOCATION "${release_lib}"
    IMPORTED_LOCATION_DEBUG "${debug_lib}"
    IMPORTED_LOCATION_RELEASE "${release_lib}"
    )
endmacro()
