cmake_minimum_required(VERSION 3.10)
project(TpOpenGl VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(FreeGLUT REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)

include_directories(
  ${GLFW_INCLUDE_PATH}
  ${GLEW_INCLUDE_PATH}
	${GLM_INCLUDE_PATH}
)
include_directories(include)

set(SOURCES
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
  FreeGLUT::freeglut
)