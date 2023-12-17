cmake_minimum_required(VERSION 3.16.3)

project(portadiva)

set(IS_DEBUG_BUILD CMAKE_BUILD_TYPE STREQUAL "Debug")

if (${IS_DEBUG_BUILD})
    add_compile_definitions(__DEBUG__)
endif ()


add_executable(${PROJECT_NAME} 
    src/main.c 
    src/game/core/dsc/dsc.c
    src/core/list.c
)

include_directories(.)