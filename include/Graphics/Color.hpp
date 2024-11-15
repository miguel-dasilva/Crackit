#ifndef GRAPHICS_COLOR_H_
#define GRAPHICS_COLOR_H_

#include <stdint.h>
#include <SDL.h>
struct SDL_PixelFormat;

enum class PieceColor {
    RED = 1,
    BLUE,
    GREEN,
    YELLOW
};

class Color
{
    uint32_t mColor;
    int mColorId;
    public:

        static const SDL_PixelFormat* mFormat;
        static void InitColorFormat(const SDL_PixelFormat* format);

        static Color Evaluate1MinusSourceAlpha(const Color& source, const Color& destination);

        static Color Black() {return Color(0, 0, 0, 255);}
        static Color White() {return Color(255, 255, 255, 255);}
        static Color Red() {return Color(255, 0, 0, 255);}
        static Color Green() {return Color(0, 255, 0, 255);}
        static Color Blue() {return Color(0, 0, 255, 255);}
        static Color Yellow() {return Color(255, 255, 0, 255);}
        static Color DarkGrey() {return Color (128, 128, 128, 255);}

        SDL_Color ToSDLColor() const;
        static Color GetColorFromId(int colorId);
        
        inline void SetColorId(int id) { mColorId = id; }
        inline int  GetColorId() {return mColorId;}

        Color(): Color(0) {}
        Color(uint32_t color): mColor(color) {}
        Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        Color(PieceColor color);

        inline bool operator==(const Color& c) const {return mColor == c.mColor;}
        inline bool operator!=(const Color& c) const {return !(*this == c);}
        inline uint32_t GetPixelColor() const { return mColor;}

        void SetRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

        void SetRed(uint8_t red);
        void SetGreen(uint8_t green);
        void SetBlue(uint8_t blue);
        void SetAlpha(uint8_t alpha);

        uint8_t GetRed() const;
        uint8_t GetGreen() const;
        uint8_t GetBlue() const;
        uint8_t GetAlpha() const;
        static PieceColor GetRandomPieceColorEnum();
};

#endif