cmake_minimum_required(VERSION 3.0)

project(opencv2opengl)


# set the flags, may want to add/change debugging or optimization (-O3) flags as appropriate
# set(CMAKE_CXX_COMPILER "/usr/bin/g++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -O3 -std=c++11 ")

find_package (Eigen3 3.3 REQUIRED NO_MODULE)
find_package(OpenGL 3.3 REQUIRED)
find_package(OpenCV REQUIRED ) 
find_package(glfw3 REQUIRED ) 

#glad
include_directories("../includes/")

include_directories("/usr/local/include/glm")

add_executable(opencv2opengl
OpenCV2OpenGL1.cpp
OpenCV2OpenGL1.hpp
includes.hpp
mesh_ply.hpp
mesh_ply.cpp
shader.cpp
shader.cpp
ReadPly.cpp
ReadPly.hpp
helpers.cpp
helpers.hpp
glad.c
)

target_link_libraries(opencv2opengl ${OPENGL_LIBRARIES} glfw3 Eigen3::Eigen ${OpenCV_LIBS} pthread dl rt X11 m Xrandr)

#optional install 
set_property(TARGET opencv2opengl PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)

install(TARGETS opencv2opengl DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )
