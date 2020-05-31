find_package(OpenGL)
include_directories( ${OPENGL_INCLUDE_DIRS} )

add_subdirectory(ThirdParty/SDL2)
add_subdirectory(ThirdParty/glew)

include_directories(ThirdParty/SDL2/include)
include_directories(ThirdParty/glew/include)

# Add glm (header only library)
include_directories(ThirdParty/glm)

add_library(Sandbox STATIC Sandbox.cpp ParticleSystem.cpp Util.cpp)
target_link_libraries(Sandbox PRIVATE SDL2
                                      $<$<PLATFORM_ID:Windows>:SDL2main>
                                      glew
                                      OpenGL::GL)

add_executable(Main Main.cpp)
target_link_libraries(Main Sandbox)
