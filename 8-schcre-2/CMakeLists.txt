# CMakeLists for Project 2

include(${CMAKE_SOURCE_DIR}/cmake/eece2560_project_utils.cmake)

# Static library for shared elements in Part A and Part B
add_library(${EECE2560_GROUP_ID}-2-lib STATIC card.h card.cpp deck.h)
eece2560_target_warning_defaults(${EECE2560_GROUP_ID}-2-lib PRIVATE)

# Executable for Part A
add_executable(${EECE2560_GROUP_ID}-2a part_a.cpp)
eece2560_target_warning_defaults(${EECE2560_GROUP_ID}-2a PRIVATE)
target_link_libraries(${EECE2560_GROUP_ID}-2a ${EECE2560_GROUP_ID}-2-lib)

# Executable for Part B
add_executable(${EECE2560_GROUP_ID}-2b part_b.cpp)
eece2560_target_warning_defaults(${EECE2560_GROUP_ID}-2b PRIVATE)
target_link_libraries(${EECE2560_GROUP_ID}-2b ${EECE2560_GROUP_ID}-2-lib)

# Test executable for static library
#add_executable(${EECE2560_GROUP_ID}-2-tests project_2_tests.cpp)
#target_link_libraries(${EECE2560_GROUP_ID}-2-tests ${EECE2560_GROUP_ID}-2-lib)
#eece2560_target_warning_defaults(${EECE2560_GROUP_ID}-2-tests PRIVATE)
