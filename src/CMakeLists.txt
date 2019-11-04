project( zephyr )

find_package( Vulkan REQUIRED )

file( GLOB SOURCE *.cpp )
file( GLOB HEADERS *.h )

add_executable( zephyr
	"${CMAKE_CURRENT_SOURCE_DIR}/../deps/"
	${SOURCE}
	${HEADERS}
)

target_link_libraries( zephyr glm_static glfw ${Vulkan_LIBRARY} )