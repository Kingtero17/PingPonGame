#include "../includes/PongGame.h"
#include "../includes/Renderer.h"
#include "../includes/InputManager.h"
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

#define COLOR_ROJO     "\x1b[31m"
#define COLOR_VERDE    "\x1b[32m"
#define COLOR_AMARILLO "\x1b[33m"
#define CP_AZUL        "\x1b[34m"
#define CP_CIAN        "\x1b[36m"
#define COLOR_RESET    "\x1b[0m"

PongGame::PongGame() 
    : currentState(GameState::MAIN_MENU), previousState(GameState::MAIN_MENU),
      lastMode(GameState::PLAYING_PVE),
      scorePlayer1(0), scorePlayer2(0),
      playerA(6, 15), playerB(113, 16), ball(62, 14, 1, 1),
      tickCounter1(0), tickCounter2(0) {}

bool PongGame::isRunning() const {
    return currentState != GameState::EXIT;
}

GameState PongGame::getState() const {
    return currentState;
}

void PongGame::resetMatch() {
    scorePlayer1 = 0;
    scorePlayer2 = 0;
    playerA = Paddle(6, 15);
    playerB = Paddle(113, 16);
    ball.reset(62, 14);
}

void PongGame::update(IInputManager& input) {
    switch (currentState) {
        case GameState::MAIN_MENU: updateMainMenu(input); break;
        case GameState::MODE_SELECT: updateModeSelect(input); break;
        case GameState::CONTROLS_MENU: updateControlsMenu(input); break;
        case GameState::PLAYING_PVE: updatePlaying(input, true); break;
        case GameState::PLAYING_PVP: updatePlaying(input, false); break;
        case GameState::PAUSED: updatePaused(input); break;
        case GameState::GAME_OVER: updateGameOver(input); break;
        case GameState::EXIT: break;
        default: break;
    }
}

// Para prevenir que una sola pulsación de tecla salte múltiples menús seguidos muy rápido 
// se usaría un debounce, pero mantendremos la lógica simple solicitada.
void PongGame::updateMainMenu(IInputManager& input) {
    if (input.isKeyDown(GameKey::ACTION1)) {
        currentState = GameState::MODE_SELECT;
#ifdef _WIN32
        Sleep(150); // Pequeño debounce rudimentario para menús
#endif
    }
    else if (input.isKeyDown(GameKey::ACTION2)) {
        currentState = GameState::CONTROLS_MENU;
#ifdef _WIN32
        Sleep(150);
#endif
    }
    else if (input.isKeyDown(GameKey::ESCAPE)) currentState = GameState::EXIT;
}

void PongGame::updateModeSelect(IInputManager& input) {
    if (input.isKeyDown(GameKey::ACTION1)) { 
        resetMatch(); 
        currentState = GameState::PLAYING_PVE;
        lastMode = GameState::PLAYING_PVE;
    }
    else if (input.isKeyDown(GameKey::ACTION2)) { 
        resetMatch(); 
        currentState = GameState::PLAYING_PVP;
        lastMode = GameState::PLAYING_PVP;
    }
    else if (input.isKeyDown(GameKey::PAUSE)) currentState = GameState::MAIN_MENU; 
}

void PongGame::updateControlsMenu(IInputManager& input) {
    if (input.isAnyKeyDown()) {
        currentState = GameState::MAIN_MENU;
#ifdef _WIN32
        Sleep(150);
#endif
    }
}

void PongGame::updatePlaying(IInputManager& input, bool isPvE) {
    if (input.isKeyDown(GameKey::PAUSE)) {
        previousState = currentState;
        currentState = GameState::PAUSED;
#ifdef _WIN32
        Sleep(150);
#endif
        return;
    }
    
    // Polling Simultáneo (ahora permite que P1 y P2 se muevan al mismo tiempo en PvP)
    if (input.isKeyDown(GameKey::UP_P1)) playerA.moveUp();
    if (input.isKeyDown(GameKey::DOWN_P1)) playerA.moveDown();
    
    if (isPvE) {
        if (input.isKeyDown(GameKey::UP_P2)) playerA.moveUp();
        if (input.isKeyDown(GameKey::DOWN_P2)) playerA.moveDown();
    } else {
        if (input.isKeyDown(GameKey::UP_P2)) playerB.moveUp();
        if (input.isKeyDown(GameKey::DOWN_P2)) playerB.moveDown();
    }

    if (!tickCounter1++) {
        if (isPvE) playerB.moveCpu(ball.getX(), ball.getY(), ball.getDx());
    }
    
    tickCounter2++;
    if (tickCounter2 >= 4) {
        ball.move(playerA, playerB);
        tickCounter2 = 0; // Reset
    }
    
    if (tickCounter1 > 1) tickCounter1 = 0;

    //Puntaje
    int score = ball.evaluateScore();
    if (score == 1) {
        scorePlayer1++;
        ball.reset(62, 14);
        ball.invertDx();
    } else if (score == 2) {
        scorePlayer2++;
        ball.reset(62, 14);
        ball.invertDx();
    }

    // Condicion Game Over
    if (scorePlayer1 >= 3 || scorePlayer2 >= 3) {
        currentState = GameState::GAME_OVER;
#ifdef _WIN32
        Sleep(150);
#endif
    }
}

void PongGame::updatePaused(IInputManager& input) {
    if (input.isKeyDown(GameKey::ACTION1)) {
        resetMatch();
        currentState = lastMode; // Reanudar jugando
    } else if (input.isKeyDown(GameKey::ACTION2)) {
        resetMatch();
        currentState = GameState::MAIN_MENU;
#ifdef _WIN32
        Sleep(150);
#endif
    } else if (input.isKeyDown(GameKey::PAUSE)) {
        currentState = previousState;
#ifdef _WIN32
        Sleep(150);
#endif
    } else if (input.isKeyDown(GameKey::ESCAPE)) {
        currentState = GameState::EXIT;
    }
}

void PongGame::updateGameOver(IInputManager& input) {
    if (input.isKeyDown(GameKey::ACTION1)) {
        resetMatch();
        currentState = lastMode;
    } else if (input.isKeyDown(GameKey::ACTION2)) {
        resetMatch();
        currentState = GameState::MAIN_MENU;
#ifdef _WIN32
        Sleep(150);
#endif
    } else if (input.isKeyDown(GameKey::ESCAPE)) {
        currentState = GameState::EXIT;
    }
}


// --- LÓGICA DE RENDERIZADO ASCII ---

void drawMarco(IRenderer& renderer) {
    for (int i = 2; i < 118; i++) {
        renderer.drawChar(i, 2, 205);
        renderer.drawChar(i, 28, 205);
    }
    for (int i = 2; i < 29; i++) {
        renderer.drawChar(2, i, 186);
        renderer.drawChar(117, i, 186);
    }
    renderer.drawChar(2, 2, 201);
    renderer.drawChar(2, 28, 200);
    renderer.drawChar(117, 2, 187);
    renderer.drawChar(117, 28, 188);
}

void PongGame::render(IRenderer& renderer) const {
    renderer.clear();
    switch (currentState) {
        case GameState::MAIN_MENU: renderMainMenu(renderer); break;
        case GameState::MODE_SELECT: renderModeSelect(renderer); break;
        case GameState::CONTROLS_MENU: renderControlsMenu(renderer); break;
        case GameState::PLAYING_PVE: 
        case GameState::PLAYING_PVP: renderPlaying(renderer); break;
        case GameState::PAUSED: renderPaused(renderer); break;
        case GameState::GAME_OVER: renderGameOver(renderer); break;
        case GameState::EXIT: {
            drawMarco(renderer);
            int x_centro = 30; 
            int y_ini = 8;
            renderer.drawString(x_centro, y_ini++, "  _____ _   _    _   _   _ _  __ ____    _____ ___  ____  ", COLOR_VERDE);
            renderer.drawString(x_centro, y_ini++, " |_   _| | | |  / \\ | \\ |  ||/ // ___|  |  ___/ _ \\|  _ \\ ", COLOR_VERDE);
            renderer.drawString(x_centro, y_ini++, "   | | | |_| | / _ \\|  \\|  |' / \\___ \\  | |_ | | | | |_) |", COLOR_VERDE);
            renderer.drawString(x_centro, y_ini++, "   | | |  _   / ___ \\  |\\  |.  \\___) |  |  _|| |_| |  _ < ", COLOR_VERDE);
            renderer.drawString(x_centro, y_ini++, "   |_| |_| |_/_/   \\_\\_| \\_|_|\\_\\____/  |_|   \\___/|_| \\_\\", COLOR_VERDE);
            
            renderer.drawString(x_centro+8, y_ini++, "  ____  _        _ __   _____ _   _  ____ ", COLOR_AMARILLO);
            renderer.drawString(x_centro+8, y_ini++, " |  _ \\| |      / \\\\ \\ / /_ _| \\ | |/ ___|", COLOR_AMARILLO);
            renderer.drawString(x_centro+8, y_ini++, " | |_) | |     / _ \\\\ V / | ||  \\| | |  _ ", COLOR_AMARILLO);
            renderer.drawString(x_centro+8, y_ini++, " |  __/| |___ / ___ \\| |  | || |\\  | |_| |", COLOR_AMARILLO);
            renderer.drawString(x_centro+8, y_ini++, " |_|   |_____/_/   \\_\\_| |___|_| \\_|\\____|", COLOR_AMARILLO);

            renderer.drawString(50, 19, "GRACIAS POR JUGAR!");
            break;
        }
        default: break;
    }
    renderer.present();
}

void PongGame::renderMainMenu(IRenderer& renderer) const {
    drawMarco(renderer);
    const char Portada[22][111] = {
    "                                                                                                              ",
    "    aaaaaaaas                                                                                                 ",
    "  aaaaaaaaaaass                                                                                               ",
    "  aaaa      aass                                                                                              ",
    "  aaaa      aass                                                                                              ",
    "  aaaa      aass         /                                                                                    ",
    "  aaaa      aass    / /   /                                             PRESIONE 1 PARA JUGAR                 ",
    "  aaaaaaaaaaaas    /   / /                                              PRESIONE 2 PARA VER CONTROLES         ",
    "  aaaaaaaaaas     / /   /                        aaaaas                 PRESIONE ESC PARA SALIR               ",
    "  aaaas          /   / /                      aaaaaaaaaas                                                     ",
    "  aaaas         aaas  /   aaas      aaas    aaa         aas                                                   ",
    "  aaaas       aaaaaaas    aaasas    aaas    aaa         aas                                                   ",
    "  aaaas       aaaaaaas    aaas aas  aaas    aaa                                                               ",
    "  aaaas         aaas      aaas  aas aaas    aaa                                                               ",
    "  aaaas                   aaas   aasaaas    aaa     aaaaass                                                   ",
    "  aaaas                   aaas    aasaas    aaa         aass                                                  ",
    "  aaaas                   aaas     aasas    aaa        aaass                                                  ",
    "  aaaas                   aaas      aass     aaaaaaaaaaaass                                                   ",
    "  aaaas                   aaas      aaas        saaaaaaas                                                     ",
    "                                                                                                              ",
    "                                                                                                              ",
    };

    for(int i = 0 ; i < 21; i++){
        for(int j = 0; j < 109; j++){
            renderer.drawChar(j + 8, i + 6, Portada[i][j], COLOR_RESET);
        }
    }
}

void PongGame::renderModeSelect(IRenderer& renderer) const {
    drawMarco(renderer);
    renderer.drawString(45, 12, "SELECCIONE MODO DE JUEGO", COLOR_AMARILLO);
    renderer.drawString(45, 15, "1. Un Jugador (Vs CPU)", COLOR_VERDE);
    renderer.drawString(45, 17, "2. Multijugador (PvP)", COLOR_VERDE);
    renderer.drawString(45, 19, "3. Volver al Menu Principal", COLOR_ROJO);
}

void PongGame::renderControlsMenu(IRenderer& renderer) const {
    drawMarco(renderer);
    int a = 33;
    renderer.drawString(a, 6, "  ____ _____   _  ___ ______ _____    __    ____ ____ ", COLOR_AMARILLO);
    renderer.drawString(a, 7, " / __// __  \\ / \\/  //_  __/ / __/   / /   / __// __/ ", COLOR_AMARILLO);
    renderer.drawString(a, 8, "/ /_ / /_ / // \\/  /  / / / / / o \\ / /__ / _/_ \\ \\  ", COLOR_AMARILLO);
    renderer.drawString(a, 9, "\\___/\\_____//_/ \\_/  /_/ /_/\\_\\___//____/ \\___//___/ ", COLOR_AMARILLO);

    int b = 45;
    renderer.drawString(b, 13, "ARRIBA:    [ W ]  o  [ ^ ]");
    renderer.drawString(b, 14, "ABAJO:     [ S ]  o  [ v ]");

    renderer.drawString(b, 15, "PAUSA:     [ 3 ]", COLOR_ROJO);
    renderer.drawString(b, 16, "SALIR:     [ ESC ]", COLOR_ROJO);

    renderer.drawString(38, 20, "PRESIONE CUALQUIER TECLA PARA VOLVER AL MENU...");
}

void PongGame::renderPlaying(IRenderer& renderer) const {
    drawMarco(renderer);
    
    // Dibujar Jugador A
    int pAy = playerA.getY();
    int pAx = playerA.getX();
    for (int y = pAy - 1; y <= pAy + 1; ++y) {
        renderer.drawChar(pAx, y, 219);
    }

    // Dibujar Jugador B
    int pBy = playerB.getY();
    int pBx = playerB.getX();
    for (int y = pBy - 1; y <= pBy + 1; ++y) {
        renderer.drawChar(pBx, y, 219);
    }

    // Dibujar Pelota
    renderer.drawChar(ball.getX(), ball.getY(), 184);

    // Dibujar Puntuacion
    renderer.drawString(40, 1, std::to_string(scorePlayer1));
    renderer.drawString(78, 1, std::to_string(scorePlayer2));
}

void PongGame::renderPaused(IRenderer& renderer) const {
    drawMarco(renderer);
    renderer.drawString(56, 7, "PAUSE");
    renderer.drawString(28, 9, "SALIR:              Presione ESC para salir.");
    renderer.drawString(28, 10, "REINICIAR PARTIDA:  Presione 1 para reiniciar la partida.");
    renderer.drawString(28, 11, "MENU DE INICIO:     Presione 2 para volver al menu de inicio.");
    renderer.drawString(28, 12, "REANUDAR PARTIDA:   Presione 3 para reanudar la partida.");
}

void PongGame::renderGameOver(IRenderer& renderer) const {
    drawMarco(renderer);
    
    bool p1_won = (scorePlayer1 >= 3);
    bool p2_won = (scorePlayer2 >= 3);
    
    if (lastMode == GameState::PLAYING_PVP) {
        if (p1_won) {
            int x = 24;
            renderer.drawString(x, 6, "     _ _   _  ___   _   ___  ____  ___     _       __      __ ___  _  _  ", COLOR_VERDE);
            renderer.drawString(x, 7, "  _ | | | | |/ __| /_\\ |   \\/  _ \\| _ \\   / |      \\ \\    / /|_ _|| \\| | ", COLOR_VERDE);
            renderer.drawString(x, 8, " | || | |_| | (_ |/ _ \\| |) | (_) |   /   | |       \\ \\/\\/ /  | | | .` | ", COLOR_VERDE);
            renderer.drawString(x, 9, " \\___/\\_____/\\___/_/ \\_\\____/\\___/|_|_\\   |_|        \\_/\\_/  |___||_|\\_| ", COLOR_VERDE);
        } else if (p2_won) {
            int x = 24;
            renderer.drawString(x, 6, "     _ _   _  ___   _   ___   ___  ___     ___      __      __ ___  _  _  ", CP_CIAN);
            renderer.drawString(x, 7, "  _ | | | | |/ __| /_\\ |   \\ / _ \\| _ \\   |_  )     \\ \\    / /|_ _|| \\| | ", CP_CIAN);
            renderer.drawString(x, 8, " | || | |_| | (_ |/ _ \\| |) | (_) |   /    / /       \\ \\/\\/ /  | | | .` | ", CP_CIAN);
            renderer.drawString(x, 9, " \\___/\\_____/\\___/_/ \\_\\____/\\___/|_|_\\   /___|       \\_/\\_/  |___||_|\\_| ", CP_CIAN);
        }
    } else { // PLAYING_PVE
        if (p1_won) {
            int x = 24;
            renderer.drawString(x, 6, "     _ _   _  ___   _   ___  ____  ___     _       __      __ ___  _  _  ", COLOR_VERDE);
            renderer.drawString(x, 7, "  _ | | | | |/ __| /_\\ |   \\/  _ \\| _ \\   / |      \\ \\    / /|_ _|| \\| | ", COLOR_VERDE);
            renderer.drawString(x, 8, " | || | |_| | (_ |/ _ \\| |) | (_) |   /   | |       \\ \\/\\/ /  | | | .` | ", COLOR_VERDE);
            renderer.drawString(x, 9, " \\___/\\_____/\\___/_/ \\_\\____/\\___/|_|_\\   |_|        \\_/\\_/  |___||_|\\_| ", COLOR_VERDE);
        } else if (p2_won) {
            int c = 34; 
            renderer.drawString(c, 6,  " _________    __  _________   ____ _   ____________ ", COLOR_ROJO);
            renderer.drawString(c, 7,  "/ ____/   |  /  |/  / ____/  / __ \\ | / / ____/ __ \\", COLOR_ROJO);
            renderer.drawString(c, 8,  "/ / __/ /| | / /|_/ / __/    / / / / |/ / __/ / /_/ /", COLOR_ROJO);
            renderer.drawString(c, 9,  "/ /_/ / ___ |/ /  / / /___   / /_/ /|  / /___/ _, _/ ", COLOR_ROJO);
            renderer.drawString(c, 10, "\\____/_/  |_/_/  /_/_____/   \\____/ |_/_____/_/ |_|  ", COLOR_ROJO);
        }
    }

    int a = 28;
    renderer.drawString(a, 13, "SALIR:              Presione ESC para salir.");
    renderer.drawString(a, 14, "REINICIAR PARTIDA:  Presione 1 para reiniciar la partida.");
    renderer.drawString(a, 15, "MENU DE INICIO:     Presione 2 para volver al menu de inicio.");
}
