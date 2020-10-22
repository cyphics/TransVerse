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
        gui.c
        gui_game.c
        gui_config.c
        gui_code.c physics.c)

set(MAIN_SOURCES main.c)
set(HEADERS  ${CONFIG_HEADER}
        game_handler.h
        game.h
        game_debug.h
        helper.h
        storage.h
        gui.h
        gui_game.c
        gui_config.c
        gui_code.c gui_game.h  gui_constants.h physics.h)

include_directories(${PROJECT_SOURCE_DIR}/src/)
add_library(game STATIC ${LIB_SOURCES} ${HEADERS})
add_executable(transverse ${HEADERS} ${MAIN_SOURCES})

target_include_directories(transverse PRIVATE ../include)
target_link_libraries(transverse m) # Add math library
target_link_libraries(transverse raylib)
target_link_libraries(transverse game)

install(TARGETS game
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION ../lib
        ARCHIVE DESTINATION ../lib)
