include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)

# If pkg-config is not installed on the system, then the
# CHECK_INSTALL_DIR variable must be set to the install
# location of Check. For example, on Windows, this may
# be: C:/Program Files/check
# set(CHECK_INSTALL_DIR "C:/Program Files/check")

find_package(Check REQUIRED)
include_directories(${CHECK_INCLUDE_DIRS})
link_directories(${CHECK_LIBRARY_DIRS})

set(TEST_SOURCES test_game.c test_storage.c test_main.c test_physics.c test_physics.h)

add_executable(test_suite ${TEST_SOURCES})
target_link_libraries(test_suite transverse_lib ${CHECK_LIBRARIES})
target_link_libraries(test_suite m) # Add math library
