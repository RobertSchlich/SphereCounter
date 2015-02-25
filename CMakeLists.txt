project(spherecounter)

add_library(spherecounter_lib STATIC sphere.cpp layer.cpp)
add_executable(spherecounter main.cpp)
target_link_libraries(spherecounter spherecounter_lib)

install(TARGETS spherecounter RUNTIME DESTINATION bin)
