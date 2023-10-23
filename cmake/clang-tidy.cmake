find_program(CLANG_TIDY clang-tidy)

if(NOT CLANG_TIDY)
    message(STATUS "Clang-Tidy was not found, static-analysis targets are inaccessible")
else()
    message(STATUS "Creating dummy Clang-Tidy config for build directory")
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/.clang-tidy "---\nChecks: '-*,bugprone-no-escape'\n...\n")

    add_custom_target(clang-tidy-check COMMAND ${CLANG_TIDY} ${GREE_LIB_SOURCES} ${GREE_APP_SOURCES} -p=${CMAKE_CURRENT_BINARY_DIR} -use-color
        COMMENT "Checking code with Clang-Tidy"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        USES_TERMINAL
    )
    add_custom_target(clang-tidy-fix COMMAND ${CLANG_TIDY} ${GREE_LIB_SOURCES} ${GREE_APP_SOURCES} -p=${CMAKE_CURRENT_BINARY_DIR} -use-color -fix-errors --format-style=file
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Applying automatic fixes to code with clang-tidy"
        USES_TERMINAL
    )
    
    message(STATUS "Clang-Tidy was found, 'clang-tidy-check' and 'clang-tidy-fix' targets can be used")
endif()

find_program(RUN_CLANG_TIDY run-clang-tidy)

if(NOT RUN_CLANG_TIDY)
    message(STATUS "Run-Clang-Tidy was not found, static-analysis targets are inaccessible")
else()
    add_custom_target(run-clang-tidy-check COMMAND ${RUN_CLANG_TIDY} -p=${CMAKE_CURRENT_BINARY_DIR} -use-color
        COMMENT "Checking code with run-clang-tidy (parallel)"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        USES_TERMINAL
    )
    add_custom_target(run-clang-tidy-fix COMMAND ${RUN_CLANG_TIDY} -p=${CMAKE_CURRENT_BINARY_DIR} -use-color -fix -format 
        COMMENT "Applying automatic fixes to code with run-clang-tidy (parallel)"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        USES_TERMINAL
    )

    message(STATUS "Run-Clang-Tidy was found, 'run-clang-tidy-check' and 'run-clang-tidy-fix' targets can be used")
endif()
