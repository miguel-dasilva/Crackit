#ifndef GRAPHICS_SCREEN_H_
#define GRAPHICS_SCREEN_H_

#include <stdint.h>
#include <string>
#include <map>
#include <vector>
#include <SDL_ttf.h>
#include "ScreenBuffer.hpp"
#include "Color.hpp"

class Vec2D;
class Line2D;
class Star;
class Triangle;
class AARectangle;
class Circle;

struct SDL_Window;
struct SDL_Surface;

class BMPImage;

class Screen 
{
    public:
        Screen();
        ~Screen();

        SDL_Window* Init(uint32_t w, uint32_t h, uint32_t mag);
        void SwapScreen();

        inline void SetClearColor(const Color& clearColor) {mClearColor = clearColor;}
        inline uint32_t Width() const {return mWidth;}
        inline uint32_t Height() const {return mHeight;}
        inline SDL_Window* GetWindow() {return moptrWindow;}

        void GetTextDimensions(std::string text, int& width, int& height, int fontSize);
        //Draw Methods here
        void Draw(int x, int y, const Color& color);
        void Draw(const Vec2D& point, const Color& color);
        void Draw(const Line2D& line, const Color& color);
        void Draw(const AARectangle& rectangle, const Color& color, bool fill = false, const Color& fillColor = Color::White());
        void Draw(const Circle& circle, const Color& color, bool fill = false, const Color& fillColor = Color::White());
        void Draw(const std::string& text, const Vec2D& position, const Color& color, int fontSize = 16);

    private:

        Screen(const Screen& screen);
        Screen& operator=(const Screen& screen);
        
        void ClearScreen();
        void FillPoly(const std::vector<Vec2D>& points, const Color& color);

        uint32_t mWidth;
        uint32_t mHeight;

        Color mClearColor;
        ScreenBuffer mBackBuffer;

        SDL_Window* moptrWindow;        
        SDL_Surface* mnoptrWindowSurface;

        std::map<int, TTF_Font*> mFontMap;
        TTF_Font* GetFontOfSize(int size);

};

#endif