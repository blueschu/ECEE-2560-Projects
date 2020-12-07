# CMakeLists for Project 4

include(${CMAKE_SOURCE_DIR}/cmake/eece2560_project_utils.cmake)

eece2560_add_project_targets(4
        LIB matrix.h sudoku_board.h
        PART_A part_a.cpp
        PART_B part_b.cpp
        RESOURCES resources)
