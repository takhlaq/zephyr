cmake_minimum_required( VERSION 3.2 FATAL_ERROR )
cmake_policy( VERSION 3.2 )

project( zephyr )

set(CMAKE_MODULE_PATH
	${CMAKE_MODULE_PATH}
	${CMAKE_MODULE_PATH}/cmake
)

set( GLFW3_ROOT "deps/glfw" )
set( GLM_ROOT_DIR "deps/glm" )

include( "cmake/compiler.cmake" )
include( "cmake/glfw.cmake" )
include( "cmake/glm.cmake" )

find_package( Vulkan REQUIRED )

include_directories( "deps" )
include_directories( "deps/glfw" )
include_directories( "deps/glm" )

add_subdirectory( "src" )
