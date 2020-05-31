add_subdirectory(ThirdParty/SDL2)
add_subdirectory(ThirdParty/glew)

include_directories(ThirdParty/SDL2/include)

find_package(OpenGL)

add_library(Sandbox STATIC Sandbox.cpp)
target_link_libraries(Sandbox PRIVATE SDL2 SDL2main glew OpenGL::GL)

add_executable(Main Main.cpp)
target_link_libraries(Main Sandbox)
