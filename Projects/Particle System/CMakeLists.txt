file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
	${CMAKE_CURRENT_SOURCE_DIR}/*
)

add_executable(Particle_System ${SRC_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRC_FILES})

target_include_directories(Particle_System
PUBLIC
	${CMAKE_SOURCE_DIR}/Linking/include
	${CMAKE_CURRENT_SOURCE_DIR}/
)

target_link_directories(Particle_System
PRIVATE
	${CMAKE_SOURCE_DIR}/Linking/lib/GLFW/
	${CMAKE_SOURCE_DIR}/Linking/lib/VulkanLib/
)

target_link_libraries(Particle_System
PRIVATE
	glfw3
	vulkan-1
)