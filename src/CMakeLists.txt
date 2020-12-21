cmake_minimum_required(VERSION 3.15)

project(TransVerse VERSION 0.1)
find_package(raylib 2.5.0 REQUIRED) # Requires at least version 2.5.0
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS "-std=c99")
set(CMAKE_C_STANDARD 11)

set(LIB_SOURCES
        game_handler.c
        game.c
        game_debug.c
        helper.c
        storage.c
        utils.c
        gui/gui.c
        gui/game/gui_game.c
        gui/gui_config.c
        gui/game/gui_code.c physics.c gui/gui_tools.c )
set(MAIN_SOURCES main.c gui/game/gui_engine.c gui/gui_utils.c gui/gui_utils.h gui/config/gui_cheat.c gui/config/gui_cheat.h)
set(HEADERS  ${CONFIG_HEADER}
        game_handler.h
        game.h
        game_debug.h
        helper.h
        storage.h
        utils.h
        gui/gui.h
        gui/game/gui_game.c
        gui/gui_config.c
        gui/game/gui_code.c gui/game/gui_game.h gui/gui_constants.h physics.h gui/gui_tools.h gui/game/gui_engine.h)

include_directories(${PROJECT_SOURCE_DIR}/src/)
add_library(transverse_lib STATIC ${LIB_SOURCES} ${HEADERS})
add_executable(transverse ${HEADERS} ${MAIN_SOURCES})

target_include_directories(transverse PRIVATE ../include)
target_link_libraries(transverse m) # Add math library
target_link_libraries(transverse raylib)
target_link_libraries(transverse transverse_lib)

install(TARGETS transverse_lib
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION ../lib
        ARCHIVE DESTINATION ../lib)
