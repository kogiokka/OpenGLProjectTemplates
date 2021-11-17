cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

# Define Project ###############################################################
project("NTOU OpenGL Template - Triangle"
    VERSION 1.24
    DESCRIPTION "A quick OpenGL and SDL2 setup for beginners."
)

# Create Binary Targets ########################################################

## The executable
add_executable(triangle-example)

## Static libraries
## These libraries will be consumed by our executable at the end of build.
add_library(imgui STATIC)
add_library(glad STATIC)

# Compiler Settings ############################################################

## Set C++ standard. Require C++17.
## Turn off compiler extensions for cross-platform portability.
set_target_properties(triangle-example imgui
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
)

## Set C standard.
set_target_properties(glad
    PROPERTIES
        C_STANDARD 11
        C_EXTENSIONS OFF
)

## Compiler flags for different compilers and different build configurations.
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    list(APPEND CompileOptions -Wall -Wextra -pedantic-errors)
    list(APPEND CompileOptions_Debug -ggdb)
    list(APPEND CompileOptions_Release)
    target_compile_definitions(triangle-example PRIVATE $<$<CONFIG:Debug>:_GLIBCXX_DEBUG>)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    list(APPEND CompileOptions -Wall -Wextra -pedantic-errors)
    list(APPEND CompileOptions_Debug)
    list(APPEND CompileOptions_Release)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    list(APPEND CompileOptions /W4 /permissive-)
    list(APPEND CompileOptions_Debug)
    list(APPEND CompileOptions_Release)
endif()

## Set the compile options of targets according to the "build configuration".
target_compile_options(triangle-example PRIVATE ${CompileOptions})

## Add compile options ${CompileOptions_Debug} if CONFIG:Debug is true.
target_compile_options(triangle-example PRIVATE $<$<CONFIG:Debug>:${CompileOptions_Debug}>)

## Add compile options ${CompileOptions_Release} if CONFIG:Release is true.
target_compile_options(triangle-example PRIVATE $<$<CONFIG:Release>:${CompileOptions_Release}>)

#[[
    A simplified explanation of generator expression

    $<CONFIG:Debug>
        This expression evaluates to 1 if the build configuration is "Debug", or else it evaluates to 0.

    $<1:Debug_Mode>
        This expression expands to "Debug_Mode". ($<condition:true_string>)

    $<0:Debug_Mode>
        This expression expands to an empty string.

    See details about generator-expressions at
        https://cmake.org/cmake/help/latest/manual/cmake-generator-expressions.7.html
]]

# Build Information ############################################################

message(STATUS)
message(STATUS "Build Information")
message(STATUS "\tHost System Name:       ${CMAKE_HOST_SYSTEM_NAME}" )
message(STATUS "\tHost System Version:    ${CMAKE_HOST_SYSTEM_VERSION}" )
message(STATUS "\tHost System Processor:  ${CMAKE_HOST_SYSTEM_PROCESSOR}" )
message(STATUS)
message(STATUS "\tC Compiler:             ${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION}")
message(STATUS "\tC Compiler Path:        ${CMAKE_C_COMPILER}")
message(STATUS)
message(STATUS "\tC++ Compiler:           ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "\tC++ Compiler Path:      ${CMAKE_CXX_COMPILER}")
message(STATUS)

if(MSVC)
    message(STATUS "\tMSVC Version: ${MSVC_VERSION}")
    message(STATUS "\tMSVC Toolset: ${MSVC_TOOLSET_VERSION}")
    message(STATUS)
endif()

if(CMAKE_GENERATOR MATCHES "Xcode")
    message(STATUS "\tXcode Version: ${XCODE_VERSION}")
    message(STATUS)
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Darwin")
    message(STATUS "\tMacOS SDK: ${CMAKE_OSX_SYSROOT}")
    message(STATUS)
endif()


# Find External Libraries ######################################################

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(glm REQUIRED)

# Include Path #################################################################

## PUBLIC keyword make the libraries share their include directories property
## with their consumer.
target_include_directories(imgui PUBLIC "ext/imgui")
target_include_directories(glad PUBLIC "ext/glad/include")
target_include_directories(triangle-example PRIVATE
    "include"
    "ext/imgui/backends"
)

# Configure File ###############################################################

## Let CMake define PROJECT_VERSION in C++ code for us, so we don't have to edit
## multiple files when bumping version.
configure_file(VERSION.hpp.in VERSION.hpp)
target_include_directories(triangle-example PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")

# Source Files #################################################################

## Collect source files and assign them to variables.
file(GLOB GLAD_Source "ext/glad/src/glad.c")
file(GLOB ImGui_Source CONFIGURE_DEPENDS
    "ext/imgui/*.cpp"
)
file(GLOB ImGui_Impl_Source
    "ext/imgui/backends/imgui_impl_opengl3.cpp"
    "ext/imgui/backends/imgui_impl_sdl.cpp"
)
file(GLOB My_Source CONFIGURE_DEPENDS
    "src/*.cpp"
    "src/gl/*.cpp"
    "src/sdl/*.cpp"
)

## Assign source files to the binary targets (triangle-example, glad, and imgui).
target_sources(glad PRIVATE ${GLAD_Source})
target_sources(imgui PRIVATE ${ImGui_Source})
target_sources(triangle-example PRIVATE
    ${My_Source}
    ${ImGui_Impl_Source} # Compile them with your source because they need SDL2 headers
)

# Link the Libraries ###########################################################

## GLAD use `dlopen` and `dlclose` functions in some environments. Link GLAD
## library with `CMAKE_DL_LIBS` and CMake will take care of it.
target_link_libraries(glad PRIVATE ${CMAKE_DL_LIBS})
target_link_libraries(triangle-example PRIVATE
    glad
    imgui
    SDL2::SDL2
    glm::glm
    OpenGL::GL
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9)
        target_link_libraries(triangle-example PRIVATE stdc++fs) # C++ filesystem
    endif()
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_link_libraries(triangle-example PRIVATE SDL2::SDL2main)
endif()

if (MINGW)
    # https://github.com/msys2/MINGW-packages/issues/6380
    add_definitions(-DSDL_MAIN_HANDLED)
endif()

# Create Symlink to Project Resources ###########################################

#[[
    The program needs to load some resources at runtime (ex. shaders). We create
    symbolic links from the directory containing the executable in the build tree
    to resources in the current source tree.

    NOTICE: Enable Developer Mode on MS Windows to create symbolic links.
]]
add_custom_command(TARGET triangle-example POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
        "${CMAKE_CURRENT_SOURCE_DIR}/shaders"
        "$<TARGET_FILE_DIR:triangle-example>/shaders"
    COMMAND ${CMAKE_COMMAND} -E create_symlink
        "${CMAKE_CURRENT_SOURCE_DIR}/res"
        "$<TARGET_FILE_DIR:triangle-example>/res"
    DEPENDS
        # Make sure the directories exist
        "${CMAKE_CURRENT_SOURCE_DIR}/shaders"
        "${CMAKE_CURRENT_SOURCE_DIR}/res"
    COMMENT "Creating symlinks to project resources..."
    VERBATIM
)


# CPack ########################################################################
#[[
    Package source files of the project with CPack module.

    Command:
        cmake --build <build-dir> --target package_source

    You'll see a ZIP file named "<project-name>-<version>-Source.zip" in your build directory.
]]
include(CMakeCPack.cmake)
