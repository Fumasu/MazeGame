#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Poco::Zip" for configuration "Release"
set_property(TARGET Poco::Zip APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Poco::Zip PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/libPocoZip.dll.a"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/libPocoZip.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS Poco::Zip )
list(APPEND _IMPORT_CHECK_FILES_FOR_Poco::Zip "${_IMPORT_PREFIX}/lib/libPocoZip.dll.a" "${_IMPORT_PREFIX}/bin/libPocoZip.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
