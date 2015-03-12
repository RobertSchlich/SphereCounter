project(spherecounter)

list( APPEND CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs")

add_executable(spherecounter main.cpp container.cpp sphere.cpp layer.cpp auxfunc.cpp)

install(TARGETS spherecounter RUNTIME DESTINATION bin)
