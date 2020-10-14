# CMakeLists for Project 2

include(${CMAKE_SOURCE_DIR}/cmake/eece2560_project_utils.cmake)

eece2560_add_project_targets(2
        LIB card.h card.cpp deck.h basic_unique.h linked_list.h
        PART_A part_a.cpp
        PART_B part_b.cpp)
