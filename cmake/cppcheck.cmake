find_program(CPPCHECK cppcheck)

if(NOT CPPCHECK)
    message(STATUS "Cppcheck was not found, static-analysis targets are inaccessible")
else()
    cmake_host_system_information(RESULT N_PROC QUERY NUMBER_OF_PHYSICAL_CORES)

    add_custom_target(cppcheck-check COMMAND ${CPPCHECK}
        --check-level=exhaustive
        --enable=all
        --error-exitcode=1
        --inline-suppr
        --language=c++
        --platform=native
        --project=${CMAKE_BINARY_DIR}/compile_commands.json
        --suppress=missingIncludeSystem
        --suppress=unknownMacro
        -i ${CMAKE_BINARY_DIR}
        -j ${N_PROC}
        COMMENT "Checking code with Cppcheck"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        USES_TERMINAL
    )
    
    message(STATUS "Cppcheck was found, 'cppcheck-check' target can be used")
endif()
