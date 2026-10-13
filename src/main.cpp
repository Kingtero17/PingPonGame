#include "../includes/PongGame.h"
#include "../includes/Renderer.h"
#include "../includes/InputManager.h"
#include <thread>
#include <chrono>

int main() {
    Renderer renderer(120, 30);
    InputManager input;
    PongGame game;

    while (game.isRunning()) {
        game.update(input);
        game.render(renderer);
        
        // El loop debe dormir un momento para controlar la velocidad
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    
    // Para renderizar la Pantalla Final
    game.render(renderer);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    return 0;
}
