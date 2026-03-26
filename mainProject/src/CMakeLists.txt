cmake_minimum_required(VERSION 3.10)

project(AstroModel)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(glfw3 3.3 REQUIRED)

set(OpenGL_GL_PREFERENCE GLVND)

find_package(OpenGL REQUIRED)

set(SOURCE_FILES
  ../main.c
  ../glad.c
  ../physic/Gravity/gravitySolver.c
  ../physic/particles/particle.c
  ../physic/rk4/rungekutta4.c
  ../physic/physicThread.c
  ../render/shader/shader.c
  ../utils/dynamicMem/dynamicMem.c
  ../utils/vectors/vector.c
  ../utils/sleep/sleep.c
  ../render/initOpenGL.c
  ../render/callbacks/callbacks.c
  ../render/camera/camera.c
)

add_executable(astroSim ${SOURCE_FILES})

target_include_directories(astroSim PRIVATE includes)

target_link_libraries(astroSim PRIVATE 
glfw 
OpenGL::GL
X11
pthread
Xrandr 
Xi 
dl
m
cglm
)

