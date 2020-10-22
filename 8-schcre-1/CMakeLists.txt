# CMakeLists for Project 1 - MasterMind Game

include(${CMAKE_SOURCE_DIR}/cmake/eece2560_project_utils.cmake)

eece2560_add_project_targets(1
        LIB code.cpp master_mind_game.cpp
        PART_A part_a.cpp
        PART_B part_b.cpp)

# Test executable for static library
add_executable(${EECE2560_GROUP_ID}-1-tests project_1_tests.cpp)
target_link_libraries(${EECE2560_GROUP_ID}-1-tests ${EECE2560_GROUP_ID}-1-lib)
eece2560_target_warning_defaults(${EECE2560_GROUP_ID}-1-tests PRIVATE)
