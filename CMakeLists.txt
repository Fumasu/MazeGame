cmake_minimum_required (VERSION 3.0)

project (MazeGame)

include_directories (
    .
    ${CMAKE_SOURCE_DIR}/include
)

set (MazeGame_VERSION_MAJOR 1)
set (MazeGame_VERSION_MINOR 0)

configure_file (
    ${CMAKE_SOURCE_DIR}/config.h.in
    ${CMAKE_SOURCE_DIR}/include/config.h
)

set (CMAKE_CXX_STANDARD 11)

set (CXX_SRCS
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/Game.cpp
    ${CMAKE_SOURCE_DIR}/src/Quad.cpp
    ${CMAKE_SOURCE_DIR}/src/Texture.cpp
    ${CMAKE_SOURCE_DIR}/src/Shader.cpp
	${CMAKE_SOURCE_DIR}/src/Level.cpp
)

set (C_SRCS
    ${CMAKE_SOURCE_DIR}/src/gl_core_3_3.c
)

set (EXECUTABLE_NAME "MazeGame")

add_executable (${EXECUTABLE_NAME} ${CXX_SRCS} ${C_SRCS})

set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_SOURCE_DIR}/cmake/Modules")

find_package (SOIL REQUIRED)
if (SOIL_FOUND)
    include_directories (${SOIL_INCLUDE_DIRS})
    target_link_libraries (${EXECUTABLE_NAME} ${SOIL_LIBRARIES})
endif (SOIL_FOUND)

find_package (OpenGL REQUIRED)
if (OPENGL_FOUND)
    add_definitions (${OPENGL_DEFINITIONS})
    target_link_libraries (${EXECUTABLE_NAME} ${OPENGL_LIBRARIES})
endif (OPENGL_FOUND)

find_package (GLM REQUIRED)
if (GLM_FOUND)
    target_link_libraries (${EXECUTABLE_NAME} ${GLM_LIBRARIES})
    include_directories (${GLM_INCLUDE_DIRS})
    add_definitions (${GLM_DEFINITIONS})
endif (GLM_FOUND)

find_package (SFML 2 COMPONENTS system window REQUIRED)
if (SFML_FOUND)
    target_link_libraries (${EXECUTABLE_NAME} ${SFML_LIBRARIES})
    include_directories (${SFML_INCLUDE_DIR})
    add_definitions (${SFML_DEFINITIONS})
endif (SFML_FOUND)

find_package (Thor REQUIRED)
if (THOR_FOUND)
    include_directories (${THOR_INCLUDE_DIR})
    target_link_libraries (${EXECUTABLE_NAME} ${THOR_LIBRARY})
endif (THOR_FOUND)
