# CMake utilities for ECEE 2560 project.

function(ecee2560_target_warning_defaults TARGET ACCESS)
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
    # TODO Look into method for discriminating between student and TA machines
    # when setting the /WX flag to avoid submissions failing to compile due to
    # benign warnings.

    # The warning configuration for the current compiler. This variable will
    # be empty if no configuration exists for the current compiler.
    set(CURRENT_COMPILER_WARNINGS ${COMPILER_WARNINGS_${CMAKE_CXX_COMPILER_ID}})

    # Print a warning if no compiler warning configuration exists for the
    # current compiler.
    if (NOT DEFINED COMPILER_WARNINGS_${CMAKE_CXX_COMPILER_ID})
        string(CONCAT WARNING_MESSAGE
                "Warning: CXX compiler ${CMAKE_CXX_COMPILER_ID} is not supported "
                "by ${CMAKE_PROJECT_NAME}. No warnings flags have been "
                "configured for target '${TARGET}'.")
        message(${WARNING_MESSAGE})
    endif ()

    target_compile_options(${TARGET} ${ACCESS} ${CURRENT_COMPILER_WARNINGS})


    # Alternatively,
    #target_compile_options(${TARGET} ${ACCESS}
    #        $<$<CXX_COMPILER_ID:GNU>:${COMPILER_WARNINGS_GNU}>
    #        $<$<CXX_COMPILER_ID:Clang>:${COMPILER_WARNINGS_Clang}>
    #        $<$<CXX_COMPILER_ID:MSVC>:${COMPILER_WARNINGS_MSVC}>
    #        )
endfunction()
