cmake_minimum_required(VERSION 3.15)
project(PongCrossPlatform VERSION 1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# --- Legacy Target ---
# Mantiene el código original funcionando tal cual.
add_executable(legacy_pong main.cpp)

# --- Refactored Target ---
# Descomentar en el siguiente paso cuando se implemente la lógica.
include_directories(includes)
add_executable(refactored_pong 
    src/main_refactored.cpp
    src/Paddle.cpp
    src/Ball.cpp
    src/PongGame.cpp
    src/InputManager.cpp
    src/Renderer.cpp
)

