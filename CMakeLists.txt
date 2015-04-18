cmake_minimum_required(VERSION 2.8.11)

project(MazeGame)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(SFML 2 COMPONENTS window system REQUIRED)
find_package(Freetype REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLM REQUIRED)
find_package(DevIL REQUIRED)
find_package(Poco COMPONENTS Foundation Util REQUIRED)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${SFML_INCLUDE_DIR}
	${FREETYPE_INCLUDE_DIRS}
	${OPENGL_INCLUDE_DIR}
	${GLM_INCLUDE_DIRS}
	${IL_INCLUDE_DIR}
	${IL_INCLUDE_DIR}/..
	${Poco_INCLUDE_DIR}
)

# Linker options
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS}")

# Library path
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS}")

# Define the C sources
set ( C_SRCS
    src/gl_core_4.0.c
)

# Define the CXX sources
set ( CXX_SRCS
    src/main.cpp
    src/Game.cpp
	src/Context.cpp
    src/States/State.cpp
    src/States/TitleScreen.cpp
    src/States/MenuScreen.cpp
	src/States/GameScreen.cpp
    src/Manager/StateManager.cpp
    src/Manager/ResourceManager.cpp
    src/Manager/TextureManager.cpp
    src/Resources/Texture.cpp
    src/Resources/Shader.cpp
    src/Resources/Text.cpp
)

set_source_files_properties(
    ${CXX_SRCS} PROPERTIES COMPILE_FLAGS 
    " -std=c++11 -g -O0 -Wall")

set_source_files_properties(
    ${C_SRCS} PROPERTIES COMPILE_FLAGS 
    " -g -O0 -Wall")

add_executable(MazeGame ${CXX_SRCS} ${C_SRCS})

target_link_libraries(MazeGame
	${OPENGL_LIBRARIES}
    ${FREETYPE_LIBRARIES}
    ${SFML_LIBRARIES}
	${IL_LIBRARIES}
	${ILU_LIBRARIES}
	${ILUT_LIBRARIES}
	${Poco_LIBRARIES}
)

install (TARGETS MazeGame DESTINATION .)
install (DIRECTORY res DESTINATION .)

include (CPack)