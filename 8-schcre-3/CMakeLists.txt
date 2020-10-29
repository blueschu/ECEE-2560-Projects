# CMakeLists for Project 3

include(${CMAKE_SOURCE_DIR}/cmake/eece2560_project_utils.cmake)

eece2560_add_project_targets(3
        LIB matrix.h dictionary.h dictionary.cpp algo_util.h ordinal_wrapping_sequence.h
            word_search_grid.h word_search_grid.cpp
        PART_A part_a.cpp
        PART_B part_b.cpp
        RESOURCES resources)
