# CMakeLists for Project 3

include(${CMAKE_SOURCE_DIR}/cmake/eece2560_project_utils.cmake)

eece2560_add_project_targets(3
        LIB grid.cpp grid.h sorting_util.h
        PART_A part_a.cpp
        PART_B part_b.cpp
        RESOURCES resources)
