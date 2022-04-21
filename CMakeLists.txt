cmake_minimum_required(VERSION 3.4)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake/Modules/")

project(UTAD-SIM01-Cloth)

add_executable(UTAD-SIM01-Cloth
    src/Cloth.cpp
    src/Cloth.h
    src/main.cpp
    src/Particle.cpp
    src/Particle.h
    src/Spring.cpp
    src/Spring.h
    src/Math/Vector3f.cpp
    src/Math/Vector3f.h
    src/Std/CMath.h
    src/Std/CStdInt.h
    )

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

target_include_directories(UTAD-SIM01-Cloth
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/src
    ${OPENGL_INCLUDE_DIR}
    ${GLUT_INCLUDE_DIR}
    )

target_link_libraries(UTAD-SIM01-Cloth
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARY}
    )

if (WIN32)
    add_custom_command(
        TARGET UTAD-SIM01-Cloth
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different
        "${PROJECT_SOURCE_DIR}/External/freeglut-3.2.1/build-vs2019/bin/$<CONFIG>/freeglut$<$<CONFIG:Debug>:d>.dll"
        "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>"
        )
endif ()
