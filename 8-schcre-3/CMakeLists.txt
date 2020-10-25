# CMakeLists for Project 3

include(${CMAKE_SOURCE_DIR}/cmake/eece2560_project_utils.cmake)

eece2560_add_project_targets(3
        LIB matrix.h dictionary.h dictionary.cpp sorting_util.h ordinal_wrapping_sequence.h
        PART_A part_a.cpp
        PART_B part_b.cpp
        RESOURCES resources)
