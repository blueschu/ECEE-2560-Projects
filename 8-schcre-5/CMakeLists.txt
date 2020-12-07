# CMakeLists for Project 5

include(${CMAKE_SOURCE_DIR}/cmake/eece2560_project_utils.cmake)

eece2560_add_project_targets(5
        LIB matrix.h
        PART_A part_a.cpp
        PART_B part_b.cpp
        RESOURCES resources)
