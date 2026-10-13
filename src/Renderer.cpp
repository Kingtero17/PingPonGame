#include "../includes/Renderer.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

Renderer::Renderer(int w, int h) : width(w), height(h) {
    Pixel emptyPixel = {' ', "\x1b[0m"};
    backBuffer.resize(height, std::vector<Pixel>(width, emptyPixel));
    frontBuffer.resize(height, std::vector<Pixel>(width, emptyPixel));
    hideCursor();
    
    // Limpiar pantalla al iniciar
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void Renderer::hideCursor() {
#ifdef _WIN32
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
#else
    std::cout << "\x1b[?25l";
#endif
}

void Renderer::setCursorPosition(int x, int y) {
#ifdef _WIN32
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwPos;
    dwPos.X = x;
    dwPos.Y = y;
    SetConsoleCursorPosition(hCon, dwPos);
#else
    std::cout << "\x1b[" << (y + 1) << ";" << (x + 1) << "H";
#endif
}

void Renderer::clear() {
    Pixel emptyPixel = {' ', "\x1b[0m"};
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            backBuffer[y][x] = emptyPixel;
        }
    }
}

void Renderer::drawChar(int x, int y, char c, const std::string& color) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        backBuffer[y][x] = {c, color};
    }
}

void Renderer::drawString(int x, int y, const std::string& str, const std::string& color) {
    for (size_t i = 0; i < str.length(); ++i) {
        drawChar(x + static_cast<int>(i), y, str[i], color);
    }
}

void Renderer::present() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // Solo repintar la consola si el pixel ha cambiado entre frames
            if (backBuffer[y][x].c != frontBuffer[y][x].c || 
                backBuffer[y][x].color != frontBuffer[y][x].color) {
                
                setCursorPosition(x, y);
                std::cout << backBuffer[y][x].color << backBuffer[y][x].c;
                frontBuffer[y][x] = backBuffer[y][x];
            }
        }
    }
    std::cout << "\x1b[0m"; // Resetear color al final
    std::cout.flush();
}
