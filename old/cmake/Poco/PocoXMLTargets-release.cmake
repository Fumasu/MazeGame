#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Poco::XML" for configuration "Release"
set_property(TARGET Poco::XML APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Poco::XML PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/libPocoXML.dll.a"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/libPocoXML.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS Poco::XML )
list(APPEND _IMPORT_CHECK_FILES_FOR_Poco::XML "${_IMPORT_PREFIX}/lib/libPocoXML.dll.a" "${_IMPORT_PREFIX}/bin/libPocoXML.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
