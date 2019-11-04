project( zephyr )

file( GLOB SOURCE *.cpp )
file( GLOB HEADERS *.h )

add_executable( zephyr
	"${CMAKE_CURRENT_SOURCE_DIR}/../deps/"
	${SOURCE}
	${HEADERS}
)

message( STATUS "VULKAN SHIT ${Vulkan_INCLUDE_DIR} ${Vulkan_LIBRARIES}" )

target_link_libraries( zephyr glm_static glfw ${Vulkan_LIBRARIES} )