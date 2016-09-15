#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Poco::Foundation" for configuration "Debug"
set_property(TARGET Poco::Foundation APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Poco::Foundation PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/libPocoFoundationd.dll.a"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/libPocoFoundationd.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS Poco::Foundation )
list(APPEND _IMPORT_CHECK_FILES_FOR_Poco::Foundation "${_IMPORT_PREFIX}/lib/libPocoFoundationd.dll.a" "${_IMPORT_PREFIX}/bin/libPocoFoundationd.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
