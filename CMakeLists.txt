cmake_minimum_required(VERSION 3.10)
project(TpOpenGl VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

find_package(OpenGL REQUIRED)
if (WIN32)
    include_directories(
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/glew/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/"
    )

    link_directories(
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/glew/lib/Release/x64"
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw/lib-mingw-w64"
    )

    file(COPY
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/glew/bin/Release/x64/glew32.dll"
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw/lib-mingw-w64/glfw3.dll"
        DESTINATION ${CMAKE_BINARY_DIR})
endif()

if (UNIX AND NOT APPLE)
    find_package(GLEW REQUIRED)
    find_package(glm CONFIG REQUIRED)
    find_package(glfw3 REQUIRED)

    include_directories(
        ${GLFW_INCLUDE_DIRS}
        ${GLEW_INCLUDE_DIRS}
        ${GLM_INCLUDE_DIRS}
    )
endif()

set(SOURCES
  src/camera.cpp
  src/main.cpp
  src/matrix.cpp
  src/map.cpp
  src/object.cpp
  src/program.cpp
  src/settings.cpp
  src/vector.cpp
)

file(COPY
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders"
    "${CMAKE_CURRENT_SOURCE_DIR}/textures"
    DESTINATION ${CMAKE_BINARY_DIR})

add_executable(opengl ${SOURCES})

if (WIN32)
    target_link_libraries(opengl
    OpenGL::GL
    glfw3
    glew32
    -lgdi32
    -luser32
    -lkernel32
    -lglu32
    )
endif()

if (UNIX AND NOT APPLE)
    target_link_libraries(opengl
    ${OPENGL_LIBRARIES}
    GLEW::GLEW 
    glm::glm
    glfw
    )
endif()