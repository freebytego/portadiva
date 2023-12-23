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
    src/engine/scene_object.c
    src/engine/generic_renderer.c
    src/engine/texture_manager.c
    
    src/game/core/dsc/opcode.c
    src/game/core/dsc/dsc.c
    src/game/core/objects/target.c
    src/game/core/objects/target_needle.c
    src/game/core/objects/target_real.c
    src/game/core/objects/target_real_renderer.c
    src/game/core/objects/rhythm_controller.c   
)

include_directories(.)

if(PSP)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        SDL2main
        SDL2
        SDL2_image
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
        cjson
        GL
    )
    create_pbp_file(
        TARGET ${PROJECT_NAME}
        ICON_PATH NULL
        BACKGROUND_PATH NULL
        PREVIEW_PATH NULL
        TITLE ${PROJECT_NAME}
    )
else()
    INCLUDE(FindPkgConfig)

    PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
    PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image)
    PKG_SEARCH_MODULE(CJSON REQUIRED libcjson)
    PKG_SEARCH_MODULE(GL REQUIRED gl)

    target_include_directories(${PROJECT_NAME} PRIVATE 
        ${SDL2_INCLUDE_DIRS}
        ${SDL2IMAGE_INCLUDE_DIRS}
        ${CJSON_INCLUDE_DIRS}
        ${GL_INCLUDE_DIRS}
    )
    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${SDL2_LIBRARIES}
        ${SDL2IMAGE_LIBRARIES}
        ${CJSON_LIBRARIES}
        m
        ${GL_LIBRARIES}
    )
endif()