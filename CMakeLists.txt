cmake_minimum_required(VERSION 3.21.1)

project(LinphoneIntegration)

add_library(${PROJECT_NAME} STATIC src/Model.cpp)

add_subdirectory(app)

set(LINPHONE_OUTPUT_DIR "${CMAKE_BINARY_DIR}/linphone-sdk/desktop/")

set(CMAKE_CXX_FLAGS "-std=c++0x")
set(CMAKE_XCODE_ATTRIBUTE_FRAMEWORK_SEARCH_PATHS /System/Library/Frameworks)

set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Wl,-F/Library/Frameworks")

target_include_directories(${PROJECT_NAME} 
    PUBLIC src
    PUBLIC linphone-sdk/desktop/include/linphone++
    PUBLIC /System/Library/Frameworks/linphone++.framework
)

target_link_directories(${PROJECT_NAME} 
    PUBLIC linphone-sdk/desktop/lib
)

target_link_libraries(${PROJECT_NAME} 
    "-framework linphone"
    "-framework linphone++"
)