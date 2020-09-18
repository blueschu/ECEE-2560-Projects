include(${CMAKE_SOURCE_DIR}/cmake/ecee2560_project_utils.cmake)

add_executable(8-schcre-1 main.cpp code.cpp master_mind_game.cpp)
ecee2560_target_warning_defaults(8-schcree-1 PRIVATE)

add_executable(8-schcre-1-tests project_1_tests.cpp code.cpp master_mind_game.cpp)
ecee2560_target_warning_defaults(8-schcre-1-tests PRIVATE)
