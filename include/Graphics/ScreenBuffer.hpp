#ifndef GRAPHICS_SCREENBUFFER_H_
#define GRAPHICS_SCREENBUFFER_H_

#include <stdint.h>

#include "Color.hpp"
struct SDL_Surface;

class ScreenBuffer
{
    public:
        ScreenBuffer();
        ScreenBuffer(const ScreenBuffer& ScreenBuffer);
        ~ScreenBuffer();

        ScreenBuffer& operator=(const ScreenBuffer& ScreenBuffer);

        void Init(uint32_t format, uint32_t width, uint32_t height);

        inline SDL_Surface* GetSurface() {return mSurface;}
        void AddSurface(SDL_Surface* src, int x, int y);

        void Clear(const Color& c = Color::Black());

        void SetPixel(const Color& color, int x, int y);
    private:
        SDL_Surface* mSurface;
        uint32_t GetIndex(int r, int c);
};

#endif