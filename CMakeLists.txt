cmake_minimum_required(VERSION 3.16.3)

project(portadiva)

set(IS_DEBUG_BUILD CMAKE_BUILD_TYPE STREQUAL "Debug")

if (${IS_DEBUG_BUILD})
    add_compile_definitions(__DEBUG__)
endif ()

add_executable(${PROJECT_NAME} 
    src/main.c 

    src/core/list.c

    src/engine/engine.c
    src/engine/game_object.c

    src/game/core/dsc/opcode.c
    src/game/core/dsc/dsc.c
    src/game/core/objects/target.c
    
)

find_package(SDL2 REQUIRED)

target_include_directories(${PROJECT_NAME} 
    PRIVATE ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${SDL2_LIBRARIES}
)

include_directories(.)

if(PSP)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        SDL2main
        SDL2
        GL
        png
        jpeg
        freetype
        m
        bz2
        png16
        z
        pspvram
        pspaudio
        pspvfpu
        pspgu
        pspge
        psphprm
        pspctrl
        pspdebug
        pspdisplay
    )
    create_pbp_file(
        TARGET ${PROJECT_NAME}
        ICON_PATH NULL
        BACKGROUND_PATH NULL
        PREVIEW_PATH NULL
        TITLE ${PROJECT_NAME}
    )
endif()