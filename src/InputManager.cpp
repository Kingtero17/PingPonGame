#include "../includes/InputManager.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <chrono>
#endif

InputManager::InputManager() {
    #ifndef _WIN32
        // Setup para Linux: modo no bloqueante y sin echo
        struct termios t;
        tcgetattr(STDIN_FILENO, &t);
        t.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &t);
        
        int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
    #endif
}

InputManager::~InputManager() {
    #ifndef _WIN32
        // Restaurar settings de terminal en Linux al salir
        struct termios t;
        tcgetattr(STDIN_FILENO, &t);
        t.c_lflag |= (ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &t);
        
        int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, flags & ~O_NONBLOCK);
    #endif
}

bool InputManager::isKeyDown(GameKey key) {
#ifdef _WIN32
    int virtualKey = 0;
    switch (key) {
        case GameKey::UP_P1:   virtualKey = 0x57; /* 'W' */ break;
        case GameKey::DOWN_P1: virtualKey = 0x53; /* 'S' */ break;
        case GameKey::UP_P2:   virtualKey = VK_UP; break;
        case GameKey::DOWN_P2: virtualKey = VK_DOWN; break;
        case GameKey::PAUSE:   virtualKey = 0x33; /* '3' */ break;
        case GameKey::ACTION1: virtualKey = 0x31; /* '1' */ break;
        case GameKey::ACTION2: virtualKey = 0x32; /* '2' */ break;
        case GameKey::ESCAPE:  virtualKey = VK_ESCAPE; break;
        default: return false;
    }
    // El bit mas significativo (0x8000) indica si esta siendo presionada AHORA
    return (GetAsyncKeyState(virtualKey) & 0x8000) != 0;

#else
    // Fallback basico para Linux usando lectura continua para evitar bloqueos simultaneos
    static bool keyStates[10] = {false};
    static auto lastUpdate = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();
    
    // Simular un "frame" expirando las teclas si paso mucho tiempo sin lectura real
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdate).count() > 15) {
        memset(keyStates, 0, sizeof(keyStates));
        lastUpdate = now;
    }

    char seq[3];
    while (read(STDIN_FILENO, &seq[0], 1) > 0) {
        if (seq[0] == 27) {
            int nread = read(STDIN_FILENO, &seq[1], 2);
            if (nread <= 0) keyStates[(int)GameKey::ESCAPE] = true;
            else if (nread == 2 && seq[1] == '[') {
                if (seq[2] == 'A') keyStates[(int)GameKey::UP_P2] = true;
                if (seq[2] == 'B') keyStates[(int)GameKey::DOWN_P2] = true;
            }
        } else {
            if (seq[0] == 'w' || seq[0] == 'W') keyStates[(int)GameKey::UP_P1] = true;
            if (seq[0] == 's' || seq[0] == 'S') keyStates[(int)GameKey::DOWN_P1] = true;
            if (seq[0] == '1') keyStates[(int)GameKey::ACTION1] = true;
            if (seq[0] == '2') keyStates[(int)GameKey::ACTION2] = true;
            if (seq[0] == '3') keyStates[(int)GameKey::PAUSE] = true;
        }
    }
    return keyStates[(int)key];
#endif
}

bool InputManager::isAnyKeyDown() {
#ifdef _WIN32
    for (int i = 8; i <= 255; i++) {
        if (GetAsyncKeyState(i) & 0x8000) {
            return true;
        }
    }
    return false;
#else
    char seq[3];
    bool pressed = false;
    while (read(STDIN_FILENO, &seq[0], 1) > 0) {
        pressed = true;
    }
    return pressed;
#endif
}
