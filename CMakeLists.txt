cmake_minimum_required(VERSION 3.10)

project(ParticleSystem)

if (WIN32)
    add_compile_definitions(GLEW_STATIC)
endif (WIN32)

set(CMAKE_CXX_STANDARD, 17)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(Source)

FILE(COPY Shaders/Particles.vert Shaders/Particles.frag DESTINATION .)

