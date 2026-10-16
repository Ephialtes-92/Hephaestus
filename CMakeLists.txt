
cmake_minimum_required(VERSION 3.23)

project(Hephaestus LANGUAGES C CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Project directories
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source")
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(THIRDPARTY_DIR "${INCLUDE_DIR}/ThirdParty")
set(LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libraries")

# Automatically discover C/C++ source files.
file(GLOB_RECURSE PROJECT_SOURCES
    CONFIGURE_DEPENDS
    "${SOURCE_DIR}/*.cpp"
    "${SOURCE_DIR}/*.c"
)

# Discover headers for Visual Studio Solution Explorer.
file(GLOB_RECURSE PROJECT_HEADERS
    CONFIGURE_DEPENDS
    "${INCLUDE_DIR}/*.h"
    "${INCLUDE_DIR}/*.hpp"
    "${INCLUDE_DIR}/*.inl"
)

# Create the application.
add_executable(Hephaestus
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
)

# Header search paths.
target_include_directories(Hephaestus PRIVATE
    "${INCLUDE_DIR}"
    "${THIRDPARTY_DIR}"
)

# OpenGL.
find_package(OpenGL REQUIRED)

target_link_libraries(Hephaestus PRIVATE
    OpenGL::GL
    "${LIBRARY_DIR}/glfw3.lib"
)

# Additional Windows system libraries used by GLFW.
if(WIN32)
    target_link_libraries(Hephaestus PRIVATE
        user32
        gdi32
        shell32
    )
endif()

# Organize files in Visual Studio.
source_group(
    TREE "${SOURCE_DIR}"
    PREFIX "Source Files"
    FILES ${PROJECT_SOURCES}
)

source_group(
    TREE "${INCLUDE_DIR}"
    PREFIX "Header Files"
    FILES ${PROJECT_HEADERS}
)
