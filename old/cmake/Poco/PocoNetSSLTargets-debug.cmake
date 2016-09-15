#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Poco::NetSSL" for configuration "Debug"
set_property(TARGET Poco::NetSSL APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Poco::NetSSL PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/libPocoNetSSLd.dll.a"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/libPocoNetSSLd.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS Poco::NetSSL )
list(APPEND _IMPORT_CHECK_FILES_FOR_Poco::NetSSL "${_IMPORT_PREFIX}/lib/libPocoNetSSLd.dll.a" "${_IMPORT_PREFIX}/bin/libPocoNetSSLd.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
