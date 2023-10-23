find_program(CLANG_FORMAT clang-format)

if(NOT CLANG_FORMAT)
    message(STATUS "Clang-Format was not found, automatic code format targets are inaccessible")
else()
    set(FILES ${GREE_LIB_HEADERS} ${GREE_LIB_SOURCES} ${GREE_APP_HEADERS} ${GREE_APP_SOURCES})

    add_custom_target(clang-format COMMAND ${CLANG_FORMAT} -i ${FILES} 
        COMMENT "Formatting code with Clang-Format"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        USES_TERMINAL
    )
    add_custom_target(clang-format-check COMMAND ${CLANG_FORMAT} --dry-run --Werror ${FILES}
        COMMENT "Checking format with Clang-Format check"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        USES_TERMINAL
    )

    message(STATUS "Clang-Format was found, 'clang-format' and 'clang-format-check' targets can be used")
endif()
