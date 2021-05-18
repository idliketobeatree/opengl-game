cmake_minimum_required(VERSION 3.5)
project(Game VERSION 1.0.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(all_source "${PROJECT_SOURCE_DIR}/src/")
set(main_source "${PROJECT_SOURCE_DIR}/src/main/")
set(util_source "${PROJECT_SOURCE_DIR}/src/util/")

set(stb_source "${PROJECT_SOURCE_DIR}/lib/stb/")

add_executable(${PROJECT_NAME} src/main/main.cpp src/engine/rendering/rendering.hpp src/engine/rendering/rendering.cpp src/engine/rendering/window.hpp src/util/logging/logging.hpp src/util/logging/logging.cpp src/engine/rendering/renderer/Renderer.hpp src/main/rendering/renderer/GameRenderer.cpp src/main/rendering/renderer/GameRenderer.hpp src/engine/rendering/shader/Shader.hpp src/engine/rendering/shader/Shader.cpp src/engine/rendering/shader/ShaderProgram.cpp src/engine/rendering/shader/ShaderProgram.hpp src/main/rendering/rendering.hpp src/main/rendering/rendering.cpp src/engine/rendering/typedefs.hpp src/engine/rendering/camera/Camera.cpp src/engine/rendering/camera/Camera.hpp src/util/math/math.hpp src/main/settings.hpp src/main/rendering/camera/GameCamera.cpp src/main/rendering/camera/GameCamera.hpp src/main/rendering/renderer/DebugRenderer.cpp src/main/rendering/renderer/DebugRenderer.hpp src/engine/world/chunk/Chunk.hpp src/engine/world/block/Block.hpp src/main/world/block/blocks.hpp src/engine/world/block/BlockType.hpp src/engine/world/chunk/Chunk.cpp src/main/rendering/renderer/ChunkRenderer.cpp src/main/rendering/renderer/ChunkRenderer.hpp src/util/math/Vector.hpp src/util/math/Matrix.hpp src/util/math/Noise.hpp src/util/math/math.cpp)
include_directories(${PROJECT_NAME} PUBLIC ${stb_source} PUBLIC ${util_source} PUBLIC ${main_source} PUBLIC ${all_source})

add_subdirectory(${PROJECT_SOURCE_DIR}/lib/glfw-3.3.4/)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/glad-0.1.34/)

target_link_libraries(${PROJECT_NAME} PUBLIC glfw PUBLIC glad)