cmake_minimum_required(VERSION 3.10)
project(TpOpenGl VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glfw3 REQUIRED)

include_directories(
  ${GLFW_INCLUDE_DIRS}
  ${GLEW_INCLUDE_DIRS}
  ${GLM_INCLUDE_DIRS}
)
include_directories(include)

set(SOURCES
  src/camera.cpp
  src/main.cpp
  src/matrix.cpp
  src/program.cpp
  src/settings.cpp
  src/vector.cpp
)

add_executable(opengl ${SOURCES})
target_link_libraries(opengl
  ${OPENGL_LIBRARIES}
  GLEW::GLEW 
  glm::glm
  glfw
)
