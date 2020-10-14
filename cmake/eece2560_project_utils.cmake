# CMake utilities for EECE 2560 project.
#
# References:
# - https://cmake.org/cmake/help/latest/command/target_compile_options.html
# - https://cmake.org/cmake/help/latest/command/cmake_parse_arguments.html

include(CMakeParseArguments)

set(EECE2560_GROUP_ID 8-schcre)

# Configures custom compiler warnings for the given target with the given
# access scope.
function(eece2560_target_warning_defaults TARGET ACCESS)
    # GCC warning flags. See https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html.
    # -Wall/Wextra  - enable common warnings.
    # -Werror       - treat warnings as errors.
    # -pedantic     - disable gcc extensions; ensure valid ISO C++.
    # -Wconversion  - warn about possibly lossy implicit conversions.
    set(COMPILER_WARNINGS_GNU -Wall -Wextra -pedantic -Werror -Wconversion)
    # Clang warning flags. We assume all GCC flags used are also supported by Clang.
    set(COMPILER_WARNINGS_Clang ${COMPILER_WARNINGS_GNU})
    # MSVC warning flags.
    # /Wall         - enable all warnings.
    # /WX           - treat warnings as errors.
    set(COMPILER_WARNINGS_MSVC /Wall /WX)

    # The warning configuration for the current compiler. This variable will
    # be empty if no configuration exists for the current compiler.
    set(CURRENT_COMPILER_WARNINGS ${COMPILER_WARNINGS_${CMAKE_CXX_COMPILER_ID}})

    # Print a warning if no compiler warning configuration exists for the
    # current compiler.
    if (NOT DEFINED COMPILER_WARNINGS_${CMAKE_CXX_COMPILER_ID})
        message(WARNING "No default warning flags available for current CXX\
                compiler '${CMAKE_CXX_COMPILER_ID}'. Cannot configure warning\
                flags for target '${TARGET}'")
    endif ()

    target_compile_options(${TARGET} ${ACCESS} ${CURRENT_COMPILER_WARNINGS})

    # Alternatively,
    #target_compile_options(${TARGET} ${ACCESS}
    #        $<$<CXX_COMPILER_ID:GNU>:${COMPILER_WARNINGS_GNU}>
    #        $<$<CXX_COMPILER_ID:Clang>:${COMPILER_WARNINGS_Clang}>
    #        $<$<CXX_COMPILER_ID:MSVC>:${COMPILER_WARNINGS_MSVC}>
    #        )
endfunction()

# Generates CMake targets for an EECE2560 project.
function(eece2560_add_project_targets PROJ_NUM)
    cmake_parse_arguments(
            PARSED         # Output variable prefix.
            ""                  # No boolean arguments.
            ""                  # No single value arguments.
            "LIB;PART_A;PART_B" # Multi-value arguments for target sources.
            ${ARGN}
    )

    set(TARGET_PREFIX "${EECE2560_GROUP_ID}-${PROJ_NUM}")

    if (NOT PARSED_LIB)
        message(FATAL_ERROR "No LIB sources given to project ${PROJ_NUM}\
            - unable to create target ${TARGET_PREFIX}-lib")
    endif ()

    if (NOT PARSED_PART_A)
        message(FATAL_ERROR "No PART_A sources given to project ${PROJ_NUM}\
            - unable to create target ${TARGET_PREFIX}a")
    endif ()

    if (NOT PARSED_PART_B)
        message(FATAL_ERROR "No PART_B sources given to project ${PROJ_NUM}\
            - unable to create target ${TARGET_PREFIX}b")
    endif ()

    # Static library for shared elements in Part A and Part B
    add_library(${TARGET_PREFIX}-lib STATIC ${PARSED_LIB})
    eece2560_target_warning_defaults(${TARGET_PREFIX}-lib PRIVATE)

    # Executable for Part A
    add_executable(${TARGET_PREFIX}a ${PARSED_PART_A})
    eece2560_target_warning_defaults(${TARGET_PREFIX}a PRIVATE)
    target_link_libraries(${TARGET_PREFIX}a ${TARGET_PREFIX}-lib)

    # Executable for Part B
    add_executable(${TARGET_PREFIX}b ${PARSED_PART_B})
    eece2560_target_warning_defaults(${TARGET_PREFIX}b PRIVATE)
    target_link_libraries(${TARGET_PREFIX}b ${TARGET_PREFIX}-lib)

endfunction()
