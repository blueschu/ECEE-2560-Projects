include(${CMAKE_SOURCE_DIR}/cmake/ecee2560_project_utils.cmake)

add_executable(8-schcree-1 main.cpp code.cpp)
ecee2560_target_warning_defaults(8-schcree-1 PRIVATE)
