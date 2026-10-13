#pragma once

#include "Paddle.h"
#include "Ball.h"

//Inyeccion de dependencias
class IRenderer;
class IInputManager;

enum class GameState {
    MAIN_MENU,
    MODE_SELECT,     // Elegir PvE o PvP
    CONTROLS_MENU,   // Ver controles
    PLAYING_PVE,     // Opcion 1
    PLAYING_PVP,     // Opcion 2 
    PAUSED,
    GAME_OVER,
    POST_GAME,      // Menu al terminar el juego (opciones 1, 2, 3 en Game Over)
    EXIT
};

class PongGame {
    private:
        GameState currentState;
        GameState previousState; // Para regresar del menu de pausa
        GameState lastMode;      // Recuerda el ultimo modo jugado (PVE o PVP)
        
        // Variables de estado del juego
        int scorePlayer1;
        int scorePlayer2;
        
        Paddle playerA;
        Paddle playerB;
        Ball ball;

        // Controladores de velocidad para el loop principal
        int tickCounter1;
        int tickCounter2;

        // Metodos de actualizacion por estado
        void updateMainMenu(IInputManager& input);
        void updateModeSelect(IInputManager& input);
        void updateControlsMenu(IInputManager& input);
        void updatePlaying(IInputManager& input, bool isPvE);
        void updatePaused(IInputManager& input);
        void updateGameOver(IInputManager& input);

        // Metodos de renderizado por estado
        void renderMainMenu(IRenderer& renderer) const;
        void renderModeSelect(IRenderer& renderer) const;
        void renderControlsMenu(IRenderer& renderer) const;
        void renderPlaying(IRenderer& renderer) const;
        void renderPaused(IRenderer& renderer) const;
        void renderGameOver(IRenderer& renderer) const;

        // Reinicia el estado de las variables (puntajes, posiciones)
        void resetMatch();

    public:
        PongGame();

        // Actualiza la logica un "tick" completo
        void update(IInputManager& input);
        
        // Dibuja el estado actual
        void render(IRenderer& renderer) const;

        [[nodiscard]] bool isRunning() const;
        [[nodiscard]] GameState getState() const;
};
