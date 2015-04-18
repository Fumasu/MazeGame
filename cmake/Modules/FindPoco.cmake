# define the list of search paths for headers and libraries
set (FIND_Poco_PATHS
	${Poco_ROOT}
	$ENV{Poco_ROOT}
	~/Library/Frameworks
	/Library/Frameworks
	/usr/local
	/usr
	/sw
	/opt/local
	/opt/csw
	/opt)

# find the Poco include directory
find_path (Poco_INCLUDE_DIR Poco/Poco.h
			PATH_SUFFIXES include
			PATHS ${FIND_Poco_PATHS})

# check the version number
set (Poco_VERSION_OK TRUE)
if (Poco_FIND_VERSION AND Poco_INCLUDE_DIR)
	# extract the major and minor version numbers from Poco/Version.h
	# we have to handle framework a little bit differently:
	if ("${Poco_INCLUDE_DIR}" MATCHES "Poco.framework")
		set (Poco_VERSION_H_INPUT "${Poco_INCLUDE_DIR}/Headers/Version.h")
	else()
		set (Poco_VERSION_H_INPUT "${Poco_INCLUDE_DIR}/Poco/Version.h")
	endif()
	
	FILE (READ "${Poco_VERSION_H_INPUT}" Poco_VERSION_H_CONTENTS)
endif()

# find the requested modules
set (Poco_FOUND TRUE)
foreach (FIND_Poco_COMPONENT ${Poco_FIND_COMPONENTS})
	string (TOLOWER ${FIND_Poco_COMPONENT} FIND_Poco_COMPONENT_LOWER)
	string (TOUPPER ${FIND_Poco_COMPONENT} FIND_Poco_COMPONENT_UPPER)
	set (FIND_Poco_COMPONENT_NAME Poco${FIND_Poco_COMPONENT})
	
	# static release library
	find_library (Poco_${FIND_Poco_COMPONENT_UPPER}_LIBRARY_STATIC_RELEASE
				  NAMES ${FIND_Poco_COMPONENT_NAME}-s
				  PATH_SUFFIXES lib64 lib
				  PATHS ${FIND_Poco_PATHS})
				  
	# static debug library
	find_library (Poco_${FIND_Poco_COMPONENT_UPPER}_LIBRARY_STATIC_DEBUG
				  NAMES ${FIND_Poco_COMPONENT_NAME}-sd
				  PATH_SUFFIXES lib64 lib
				  PATHS ${FIND_Poco_PATHS})
				  
	# dynamic release library
	find_library (Poco_${FIND_Poco_COMPONENT_UPPER}_LIBRARY_DYNAMIC_RELEASE
				  NAMES ${FIND_Poco_COMPONENT_NAME}
				  PATH_SUFFIXES lib64 lib
				  PATHS ${FIND_Poco_PATHS})
				  
	# dynamic debug library
	find_library (Poco_${FIND_Poco_COMPONENT_UPPER}_LIBRARY_DYNAMIC_DEBUG
				  NAMES ${FIND_Poco_COMPONENT_NAME}
				  PATH_SUFFIXES lib64 lib
				  PATHS ${FIND_Poco_PATHS})
				  
	# choose the entries that fit the requested link type
	if (Poco_STATIC_LIBRARIES)
		if (Poco_${FIND_Poco_COMPONENT_UPPER}_LIBRARY_STATIC_RELEASE)
			set (Poco_${FIND_Poco_COMPONENT_UPPER}_LIBRARY_RELEASE ${Poco_${FIND_Poco_COMPONENT_UPPER}_LIBRARY_STATIC_RELEASE})
		endif()
		if (Poco_${FIND_Poco_COMPONENT_UPPER}_LIBRARY_STATIC_DEBUG)
			set (Poco_${FIND_Poco_COMPONENT_UPPER}_LIBRARY_DEBUG ${Poco_${FIND_Poco_COMPONENT_UPPER}_LIBRARY_STATIC_DEBUG})
		endif()
	else()
		if (Poco_${FIND_Poco_COMPONENT_UPPER}_LIBRARY_DYNAMIC_RELEASE)
			set (Poco_${FIND_Poco_COMPONENT_UPPER}_LIBRARY_RELEASE ${Poco_${FIND_Poco_COMPONENT_UPPER}_LIBRARY_DYNAMIC_RELEASE})
		endif()
		if (Poco_${FIND_Poco_COMPONENT_UPPER}_LIBRARY_DYNAMIC_DEBUG)
			set (Poco_${FIND_Poco_COMPONENT_UPPER}_LIBRARY_DEBUG ${Poco_${FIND_Poco_COMPONENT_UPPER}_LIBRARY_DYNAMIC_DEBUG})
		endif()
	endif()
	
	if (Poco_${FIND_Poco_COMPONENT_UPPER}_LIBRARY_DEBUG OR Poco_${FIND_Poco_COMPONENT_UPPER}_LIBRARY_RELEASE)
		#library found
		set (Poco_${FIND_Poco_COMPONENT_UPPER}_FOUND TRUE)
		
		#if both are found, set Poco_XXX_LIBRARY to contain both
		if (Poco_${FIND_Poco_COMPONENT_UPPER}_LIBRARY_DEBUG AND Poco_${FIND_Poco_COMPONENT_UPPER}_LIBRARY_RELEASE)
			set (Poco_${FIND_Poco_COMPONENT_UPPER}_LIBRARY debug     ${Poco_${FIND_Poco_COMPONENT_UPPER}_LIBRARY_DEBUG}
														   optimized ${Poco_${FIND_Poco_COMPONENT_UPPER}_LIBRARY_RELEASE})
		endif()
		
		#if only one debug/release variant is found, set the other to be equal to the found one
		if (Poco_${FIND_Poco_COMPONENT_UPPER}_LIBRARY_DEBUG AND NOT Poco_${FIND_Poco_COMPONENT_UPPER}_LIBRARY_RELEASE)
			#debug and not release
			set (Poco_${FIND_Poco_COMPONENT_UPPER}_LIBRARY_RELEASE ${Poco_${FIND_Poco_COMPONENT_UPPER}_LIBRARY_DEBUG})
			set (Poco_${FIND_Poco_COMPONENT_UPPER}_LIBRARY         ${Poco_${FIND_Poco_COMPONENT_UPPER}_LIBRARY_DEBUG})
		endif()
		if (Poco_${FIND_Poco_COMPONENT_UPPER}_LIBRARY_RELEASE AND NOT Poco_${FIND_Poco_COMPONENT_UPPER}_LIBRARY_DEBUG)
			#release and not debug
			set (Poco_${FIND_Poco_COMPONENT_UPPER}_LIBRARY_DEBUG ${Poco_${FIND_Poco_COMPONENT_UPPER}_LIBRARY_RELEASE})
			set (Poco_${FIND_Poco_COMPONENT_UPPER}_LIBRARY       ${Poco_${FIND_Poco_COMPONENT_UPPER}_LIBRARY_RELEASE})
		endif()
	else()
		#library not found
		set (Poco_FOUND FALSE)
		set (Poco_${FIND_Poco_COMPONENT_UPPER}_FOUND FALSE)
		set (Poco_${FIND_Poco_COMPONENT_UPPER}_LIBRARY "")
		set (FIND_Poco_MISSING "${FIND_Poco_MISSING} Poco_${FIND_Poco_COMPONENT_UPPER}_LIBRARY")
	endif()
	
	#mark as advanced
	MARK_AS_ADVANCED (Poco_${FIND_Poco_COMPONENT_UPPER}_LIBRARY
					  Poco_${FIND_Poco_COMPONENT_UPPER}_LIBRARY_DEBUG
					  Poco_${FIND_Poco_COMPONENT_UPPER}_LIBRARY_RELEASE
					  Poco_${FIND_Poco_COMPONENT_UPPER}_LIBRARY_STATIC_RELEASE
					  Poco_${FIND_Poco_COMPONENT_UPPER}_LIBRARY_STATIC_DEBUG
					  Poco_${FIND_Poco_COMPONENT_UPPER}_LIBRARY_DYNAMIC_RELEASE
					  Poco_${FIND_Poco_COMPONENT_UPPER}_LIBRARY_DYNAMIC_DEBUG)

	# add to the global list of libraries
	set (Poco_LIBRARIES ${Poco_LIBRARIES} "${Poco_${FIND_Poco_COMPONENT_UPPER}_LIBRARY}")
endforeach()

# in case of static linking, we must also define the list of all dependencies of Poco libraries
# TODO

# handle errors
if (NOT Poco_VERSION_OK)
	# Poco version not ok
	set (FIND_Poco_ERROR "Poco found but version too low (requested: ${Poco_FIND_VERSION}, found: ${Poco_VERSION_MAJOR}./Poco_VERSION_MINOR}.${Poco_VERSION_PATCH})")
	set (Poco_FOUND FALSE)
elseif (NOT Poco_FOUND)
	# include directory or library not found
	set (FIND_Poco_ERROR "Could NOT find Poco (missing: ${FIND_Poco_MISSING})")
endif()
if (NOT Poco_FOUND)
	if (Poco_FIND_REQUIRED)
		# fatal error
		message (FATAL_ERROR ${FIND_Poco_ERROR})
	elseif (NOT Poco_FIND_QUIETLY)
		# error but continue
		message ("${FIND_Poco_ERROR}")
	endif()
endif()

# handle success
if (Poco_FOUND)
	message (STATUS "Found Poco ${Poco_VERSION_MAJOR}.${Poco_VERSION_MINOR}.${Poco_VERSION_PATCH} in ${Poco_INCLUDE_DIR}")
endif()