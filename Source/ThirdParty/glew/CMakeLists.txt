if (WIN32)
    add_compile_definitions(GLEW_STATIC)
endif (WIN32)

include_directories(include)
add_library(glew STATIC src/glew.c src/glewinfo.c src/visualinfo.c)
