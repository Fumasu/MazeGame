#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Poco::SevenZip" for configuration "Release"
set_property(TARGET Poco::SevenZip APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Poco::SevenZip PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/libPocoSevenZip.dll.a"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/libPocoSevenZip.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS Poco::SevenZip )
list(APPEND _IMPORT_CHECK_FILES_FOR_Poco::SevenZip "${_IMPORT_PREFIX}/lib/libPocoSevenZip.dll.a" "${_IMPORT_PREFIX}/bin/libPocoSevenZip.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
