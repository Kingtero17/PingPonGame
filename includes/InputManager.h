#pragma once

enum class GameKey {
    NONE,
    UP_P1,   // 'W' or 'w'
    DOWN_P1, // 'S' or 's'
    UP_P2,   // UP ARROW
    DOWN_P2, // DOWN ARROW
    PAUSE,   // '3'
    ACTION1, // '1'
    ACTION2, // '2'
    ESCAPE   // ESC (27)
};

class IInputManager {
    public:
        virtual ~IInputManager() = default;
        virtual bool isKeyDown(GameKey key) = 0;
        virtual bool isAnyKeyDown() = 0;
};

class InputManager : public IInputManager {
    public:
        InputManager();
        ~InputManager();
        bool isKeyDown(GameKey key) override;
        bool isAnyKeyDown() override;
};
