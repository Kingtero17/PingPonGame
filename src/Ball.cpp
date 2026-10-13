#include "../includes/Ball.h"

Ball::Ball(int startX, int startY, int startDx, int startDy)
    : x(startX), y(startY), dx(startDx), dy(startDy) {}

void Ball::move(const Paddle& playerA, const Paddle& playerB) {
    x += dx;
    y += dy;

    // Rebote en los muros
    if (y + dy == 2 || y + dy == 28) {
        dy = -dy;
    }

    // Rebote de la pelota con los jugadores o cpu
    if (x + dx == playerA.getX() && y >= playerA.getY() - 1 && y <= playerA.getY() + 1) {
        dx = -dx;
    }
    if (x + dx == playerB.getX() && y >= playerB.getY() - 1 && y <= playerB.getY() + 1) {
        dx = -dx;
    }
}

void Ball::reset(int startX, int startY) {
    x = startX;
    y = startY;
}

int Ball::evaluateScore() const {
    if (x + dx == 116) {
        return 1; // Punto para Jugador 1
    }
    if (x + dx == 3) {
        return 2; // Punto para Jugador 2
    }
    return 0; // Nadie anotó
}

void Ball::invertDx() {
    dx = -dx;
}

int Ball::getX() const { return x; }
int Ball::getY() const { return y; }
int Ball::getDx() const { return dx; }
int Ball::getDy() const { return dy; }
