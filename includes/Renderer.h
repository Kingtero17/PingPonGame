#pragma once
#include <vector>
#include <string>

struct Pixel {
    char c;
    std::string color;
};

class IRenderer {
    public:
        virtual ~IRenderer() = default;
        virtual void clear() = 0;
        virtual void drawChar(int x, int y, char c, const std::string& color = "\x1b[0m") = 0;
        virtual void drawString(int x, int y, const std::string& str, const std::string& color = "\x1b[0m") = 0;
        virtual void present() = 0;
    };

class Renderer : public IRenderer {
    private:
        int width;
        int height;
        std::vector<std::vector<Pixel>> backBuffer;
        std::vector<std::vector<Pixel>> frontBuffer;

        void hideCursor();
        void setCursorPosition(int x, int y);

    public:
        Renderer(int w = 120, int h = 30);
        
        void clear() override;
        void drawChar(int x, int y, char c, const std::string& color = "\x1b[0m") override;
        void drawString(int x, int y, const std::string& str, const std::string& color = "\x1b[0m") override;
        void present() override;
};
