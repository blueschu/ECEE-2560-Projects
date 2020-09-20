# CMakeLists for Project 1 - MasterMind Game

include(${CMAKE_SOURCE_DIR}/cmake/eece2560_project_utils.cmake)

# Static library for shared elements in Part A and Part B
add_library(${EECE2560_GROUP_ID}-1-lib STATIC
        code.cpp master_mind_game.cpp)
eece2560_target_warning_defaults(${EECE2560_GROUP_ID}-1-lib PRIVATE)

# Executable for Part A
add_executable(${EECE2560_GROUP_ID}-1a part_a.cpp)
eece2560_target_warning_defaults(${EECE2560_GROUP_ID}-1a PRIVATE)
target_link_libraries(${EECE2560_GROUP_ID}-1a ${EECE2560_GROUP_ID}-1-lib)

# Executable for Part B
add_executable(${EECE2560_GROUP_ID}-1b part_b.cpp)
eece2560_target_warning_defaults(${EECE2560_GROUP_ID}-1b PRIVATE)
target_link_libraries(${EECE2560_GROUP_ID}-1b ${EECE2560_GROUP_ID}-1-lib)

# Test executable for static library
add_executable(${EECE2560_GROUP_ID}-1-tests project_1_tests.cpp)
target_link_libraries(${EECE2560_GROUP_ID}-1-tests ${EECE2560_GROUP_ID}-1-lib)
eece2560_target_warning_defaults(${EECE2560_GROUP_ID}-1-tests PRIVATE)
