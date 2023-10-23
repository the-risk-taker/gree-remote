set(GREE_LIB_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/device.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/devicedescriptor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/devicefinder.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/greelib.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/crypto.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/crypto.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/devicefinder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/greelib.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/protocolutils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/protocolutils.h
)

add_library(greelib STATIC ${GREE_LIB_SOURCES})
target_include_directories(greelib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(greelib PRIVATE cryptopp Qt${QT_VERSION_MAJOR}::Network)
