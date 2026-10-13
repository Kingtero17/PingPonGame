#include "../includes/Paddle.h"

Paddle::Paddle(int startX, int startY) : x(startX), y(startY) {}

void Paddle::moveUp() {
    if (y > 4) {
        y -= 1;
    }
}

void Paddle::moveDown() {
    if (y < 26) {
        y += 1;
    }
}

void Paddle::moveCpu(int ballX, int ballY, int ballDx) {
    if (ballX > 70 && ballDx > 0) {
        if (ballY > y && y + 1 < 27) {
            y++;
        }
        if (ballY < y && y - 1 > 3) {
            y--;
        }
    }
}

int Paddle::getX() const { return x; }
int Paddle::getY() const { return y; }
