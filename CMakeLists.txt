cmake_minimum_required(VERSION 3.20)
project(gree)

# generic
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    add_compile_options (-fdiagnostics-color=always)
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    add_compile_options (-fcolor-diagnostics)
endif ()

add_compile_options(-Wall -Wextra -pedantic -Wno-unused-function -Werror)

# qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Network Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Network Widgets REQUIRED)

# cryptpplib
add_custom_target(build-cryptopplib COMMAND ${BASH} ${CMAKE_SOURCE_DIR}/3rdparty/./linux_compile_cryptopp.sh
    COMMENT "Building cryptopp library"
    USES_TERMINAL
)

set(CRYPTOPP_LIB_LOCATION ${CMAKE_SOURCE_DIR}/3rdparty/cryptopp/libcryptopp.a)

if(NOT EXISTS ${CRYPTOPP_LIB_LOCATION})
    message(STATUS "You should build cryptopp using 'build-cryptopplib' target first")
endif()

add_library(cryptopp STATIC IMPORTED)
set_target_properties(cryptopp PROPERTIES IMPORTED_LOCATION ${CRYPTOPP_LIB_LOCATION})
set_target_properties(cryptopp PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/3rdparty/cryptopp)

# greelib
add_subdirectory(GreeLib)

# greeapp
add_subdirectory(GreeRemoteGuiQt)

