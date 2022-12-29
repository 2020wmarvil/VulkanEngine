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

# list include directories
target_include_directories(${ENGINE_NAME} PRIVATE 
	"${ENGINE_INCLUDE_DIRS}"
	"${ENGINE_THIRDPARTY_DIR}/glfw/include"
	"${ENGINE_THIRDPARTY_DIR}/glm/glm"

    ${Vulkan_INCLUDE_DIRS}
)

# build thirdparty resources
add_subdirectory("${ENGINE_THIRDPARTY_DIR}/glfw")
add_subdirectory("${ENGINE_THIRDPARTY_DIR}/glm")

# make sure the linker can find the Lib library once it is built. 
target_link_directories(${ENGINE_NAME} PRIVATE ${LIB_BINARY_DIR}/Lib) 

# link the executable to the lib library.
target_link_libraries(${ENGINE_NAME} ${Vulkan_LIBRARY} glfw glm)