project(spherecounter)

list( APPEND CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs")

add_library(spherecounter_lib STATIC container.cpp sphere.cpp layer.cpp auxfunc.cpp)
add_executable(spherecounter main.cpp)
target_link_libraries(spherecounter spherecounter_lib)

install(TARGETS spherecounter RUNTIME DESTINATION bin)
