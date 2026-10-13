#pragma once

class Paddle {
    private:
        int x;
        int y;

    public:
        Paddle(int startX, int startY);

        void moveUp();
        void moveDown();
        void moveCpu(int ballX, int ballY, int ballDx);

        [[nodiscard]] int getX() const;
        [[nodiscard]] int getY() const;
};
