set(GREE_APP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deviceitem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deviceitem.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deviceviewmodel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/deviceviewmodel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mainwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mainwindow.h
)

add_executable(greeapp ${GREE_APP_SOURCES})
target_include_directories(greeapp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(greeapp PRIVATE greelib cryptopp Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network)
