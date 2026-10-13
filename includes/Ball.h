#pragma once

#include "Paddle.h"

class Ball {
    private:
        int x;
        int y;
        int dx;
        int dy;

    public:
        Ball(int startX, int startY, int startDx, int startDy);
        
        void move(const Paddle& playerA, const Paddle& playerB);
        void reset(int startX, int startY);
        [[nodiscard]] int evaluateScore() const;
        void invertDx();

        [[nodiscard]] int getX() const;
        [[nodiscard]] int getY() const;
        [[nodiscard]] int getDx() const;
        [[nodiscard]] int getDy() const;
};
