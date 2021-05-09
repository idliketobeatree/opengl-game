cmake_minimum_required(VERSION 3.5)
project(Game VERSION 1.0.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(all_source "${PROJECT_SOURCE_DIR}/src/")
set(main_source "${PROJECT_SOURCE_DIR}/src/main/")

set(stb_source "${PROJECT_SOURCE_DIR}/lib/stb/")

add_executable(${PROJECT_NAME} src/main/main.cpp src/engine/rendering/rendering.hpp src/engine/rendering/rendering.cpp src/engine/rendering/window.hpp src/engine/logging/logging.hpp src/engine/logging/logging.cpp src/engine/rendering/renderer/Renderer.hpp src/main/rendering/renderer/GameRenderer.cpp src/main/rendering/renderer/GameRenderer.hpp src/engine/rendering/shader/Shader.hpp src/engine/rendering/shader/Shader.cpp src/engine/rendering/shader/ShaderProgram.cpp src/engine/rendering/shader/ShaderProgram.hpp src/main/rendering/rendering.hpp src/main/rendering/rendering.cpp src/engine/rendering/typedefs.hpp src/engine/rendering/camera/Camera.cpp src/engine/rendering/camera/Camera.hpp src/engine/math/math.hpp src/main/settings.hpp src/main/rendering/camera/GameCamera.cpp src/main/rendering/camera/GameCamera.hpp)
include_directories(${PROJECT_NAME} PUBLIC ${stb_source} PUBLIC ${main_source} PUBLIC ${all_source})

add_subdirectory(${PROJECT_SOURCE_DIR}/lib/glfw-3.3.4/)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/glad-0.1.34/)

target_link_libraries(${PROJECT_NAME} PUBLIC glfw PUBLIC glad)