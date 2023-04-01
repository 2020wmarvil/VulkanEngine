cmake_minimum_required(VERSION 3.20)

# set the project name
project(Vulkan_Engine VERSION 0.0.1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set global cmake vars
set(ENGINE_NAME ${PROJECT_NAME})
set(ENGINE_THIRDPARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Thirdparty")

# set startup project for sln files
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${ENGINE_NAME})

# find and link vulkan
find_package(Vulkan)

IF (NOT Vulkan_FOUND)
	message(FATAL_ERROR "Could not find Vulkan library!")
ELSE()
	message(STATUS ${Vulkan_LIBRARY})
ENDIF()

# glob source files
file (GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp")
file (GLOB_RECURSE ENGINE_HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.h")
file (GLOB_RECURSE ENGINE_SHADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/Shaders/*.vert" "${CMAKE_CURRENT_SOURCE_DIR}/Shaders/*.frag")

set (ENGINE_INCLUDE_DIRS "")
foreach (_headerFile ${ENGINE_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND ENGINE_INCLUDE_DIRS ${_dir})
endforeach()
list (REMOVE_DUPLICATES ENGINE_INCLUDE_DIRS)

# build application executable
add_executable(${ENGINE_NAME} ${ENGINE_SOURCES} ${ENGINE_HEADERS} ${ENGINE_SHADERS})

# preprocessor definitions
target_compile_definitions(${ENGINE_NAME} PUBLIC 
	$<$<CONFIG:Debug>:_DEBUG>
)

# list include directories
target_include_directories(${ENGINE_NAME} PRIVATE 
	"${ENGINE_INCLUDE_DIRS}"
	"${ENGINE_THIRDPARTY_DIR}/VulkanMemoryAllocator/include"
	"${ENGINE_THIRDPARTY_DIR}/glfw/include"
	"${ENGINE_THIRDPARTY_DIR}/glm/glm"
	"${ENGINE_THIRDPARTY_DIR}/stb_image"
	"${ENGINE_THIRDPARTY_DIR}/tinyobjloader"
	"${ENGINE_THIRDPARTY_DIR}/vk-bootstrap"
	"${Vulkan_INCLUDE_DIRS}"
)

# build and link thirdparty resources
add_subdirectory("${ENGINE_THIRDPARTY_DIR}/glfw")
add_subdirectory("${ENGINE_THIRDPARTY_DIR}/glm")
add_subdirectory("${ENGINE_THIRDPARTY_DIR}/VulkanMemoryAllocator")
add_subdirectory("${ENGINE_THIRDPARTY_DIR}/vk-bootstrap")
target_link_libraries(${ENGINE_NAME} ${Vulkan_LIBRARY} glfw glm VulkanMemoryAllocator vkbootstrap)

# create a shader target
find_program(GLSL_VALIDATOR glslangValidator HINTS /usr/bin /usr/local/bin $ENV{VULKAN_SDK}/Bin/ $ENV{VULKAN_SDK}/Bin32/)

file(GLOB_RECURSE GLSL_SOURCE_FILES
    "${PROJECT_SOURCE_DIR}/shaders/*.frag"
    "${PROJECT_SOURCE_DIR}/shaders/*.vert"
    "${PROJECT_SOURCE_DIR}/shaders/*.comp"
    )

foreach(GLSL ${GLSL_SOURCE_FILES})
  message(STATUS "BUILDING SHADER")
  get_filename_component(FILE_NAME ${GLSL} NAME)
  set(SPIRV "${PROJECT_SOURCE_DIR}/Shaders/${FILE_NAME}.spv")
  message(STATUS ${GLSL})
  add_custom_command(
    OUTPUT ${SPIRV}
    COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
    DEPENDS ${GLSL})
  list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

add_custom_target(
    Shaders 
    DEPENDS ${SPIRV_BINARY_FILES}
    )