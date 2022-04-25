cmake_minimum_required(VERSION 3.21.1)

project(linphoneApp)

find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME} MACOSX_BUNDLE src/main.cpp src/Editor.cpp src/Application.cpp)

set(CMAKE_CXX_FLAGS "-std=c++0x")
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Wl,-F/Library/Frameworks")

#========================================================================

set(IMGUI_PATH  "modules/imgui")
file(GLOB IMGUI_SOURCES ${IMGUI_PATH}/*.cpp 
    modules/imgui/backends/imgui_impl_glfw.cpp
    modules/imgui/backends/imgui_impl_opengl2.cpp
    modules/imgui/backends/imgui_impl_opengl3.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCES})

target_include_directories(imgui 
    PUBLIC ${IMGUI_PATH}
    PUBLIC ../glad/include
    PUBLIC . .. ../imgui/backends
)

target_compile_definitions(imgui PUBLIC -DIMGUI_IMPL_OPENGL_LOADER_GLAD)
target_link_libraries(imgui PRIVATE glad glfw)

#========================================================================

add_subdirectory(modules/glad)
add_subdirectory(modules/glfw)

target_include_directories(${PROJECT_NAME} 
    PUBLIC src
    PUBLIC modules/glad/include/glad    
    PUBLIC modules/glfw/include
    PUBLIC modules/imgui
    PUBLIC modules/imgui/backends
    PUBLIC ../src
)

target_link_directories(${PROJECT_NAME} 
    PRIVATE modules/glad/src
    PRIVATE modules/glfw/src
    PRIVATE modules/imgui
    PRIVATE ../build
)

target_link_libraries(${PROJECT_NAME}
    ${OPENGL_gl_LIBRARY}
    imgui
    glad
    glfw
    LinphoneIntegration
    "-framework linphone"
    "-framework linphone++"
)