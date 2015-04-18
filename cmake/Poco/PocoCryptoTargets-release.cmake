#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Poco::Crypto" for configuration "Release"
set_property(TARGET Poco::Crypto APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Poco::Crypto PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/libPocoCrypto.dll.a"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/libPocoCrypto.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS Poco::Crypto )
list(APPEND _IMPORT_CHECK_FILES_FOR_Poco::Crypto "${_IMPORT_PREFIX}/lib/libPocoCrypto.dll.a" "${_IMPORT_PREFIX}/bin/libPocoCrypto.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
