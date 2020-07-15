cmake_minimum_required(VERSION 3.15)

project(TransVerse VERSION 0.1)
find_package(raylib 2.5.0 REQUIRED) # Requires at least version 2.5.0
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS "-std=c99")
set(CMAKE_C_STANDARD 11)
# find_package(Catch2 REQUIRED)

file(GLOB SOURCES "src/main.c")
#file(GLOB TEST_SOURCES "test/*.cpp")


add_executable(TransVerse ${SOURCES})
target_include_directories(TransVerse PRIVATE include)
target_link_libraries(TransVerse m) # Add math library
target_link_libraries(TransVerse raylib)


# Catch unit test framework
# set(testing_sources ${SOURCES})
# list(REMOVE_ITEM testing_sources ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
# add_executable(tests ${TEST_SOURCES}  ${testing_sources})
# target_include_directories(tests PRIVATE include)
# target_link_libraries(tests Catch2::Catch2)
# include(CTest)
# include(Catch)
# catch_discover_tests(tests)
# enable_testing()
