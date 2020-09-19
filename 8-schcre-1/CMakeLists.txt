include(${CMAKE_SOURCE_DIR}/cmake/ecee2560_project_utils.cmake)

# Static library for shared elements in Part A and Part B
add_library(${ECEE2560_GROUP_ID}-1-lib STATIC
        code.cpp master_mind_game.cpp)
ecee2560_target_warning_defaults(${ECEE2560_GROUP_ID}-1-lib PRIVATE)

# Executable for Part A
add_executable(${ECEE2560_GROUP_ID}-1a part_a.cpp)
ecee2560_target_warning_defaults(${ECEE2560_GROUP_ID}-1a PRIVATE)
target_link_libraries(${ECEE2560_GROUP_ID}-1a ${ECEE2560_GROUP_ID}-1-lib)

# Executable for Part B
add_executable(${ECEE2560_GROUP_ID}-1b part_b.cpp)
ecee2560_target_warning_defaults(${ECEE2560_GROUP_ID}-1b PRIVATE)
target_link_libraries(${ECEE2560_GROUP_ID}-1b ${ECEE2560_GROUP_ID}-1-lib)

# Test executable for static library
add_executable(${ECEE2560_GROUP_ID}-1-tests project_1_tests.cpp)
target_link_libraries(${ECEE2560_GROUP_ID}-1-tests ${ECEE2560_GROUP_ID}-1-lib)
ecee2560_target_warning_defaults(${ECEE2560_GROUP_ID}-1-tests PRIVATE)
