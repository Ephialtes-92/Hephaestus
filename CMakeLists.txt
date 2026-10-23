
cmake_minimum_required(VERSION 3.23)

project(Hephaestus LANGUAGES C CXX)

include(FetchContent)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Project directories
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source")
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(THIRDPARTY_DIR "${INCLUDE_DIR}/ThirdParty")
set(LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libraries")

# Asset directories (relative to source root)
set(SHADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/shaders")
set(TEXTURES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/data/textures")

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

# Configure the config header file with asset directories
configure_file(
    "${INCLUDE_DIR}/config.h.in"
    "${CMAKE_BINARY_DIR}/config.h"
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
    "${CMAKE_BINARY_DIR}"
)

# OpenGL.
find_package(OpenGL REQUIRED)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.91.9b
)
FetchContent_MakeAvailable(imgui)

add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
    ${THIRDPARTY_DIR}
)

target_compile_definitions(imgui PRIVATE
    IMGUI_IMPL_OPENGL_LOADER_GLAD
)

target_link_libraries(Hephaestus PRIVATE
    OpenGL::GL
    imgui
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
