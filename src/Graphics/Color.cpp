#include "Color.hpp"
#include <SDL.h>
#include <random>



const SDL_PixelFormat* Color::mFormat = nullptr;

Color::Color(PieceColor color) {
    SetColorId(static_cast<int>(color));
    switch (color) {
        case PieceColor::RED:
            *this = Red();
            mColorId = 1;
            break;
        case PieceColor::BLUE:
            *this = Blue();
            mColorId = 2;
            break;
        case PieceColor::GREEN:
            *this = Green();
            mColorId = 3;
            break;
        case PieceColor::YELLOW:
            *this = Yellow();
            mColorId = 4;
            break;
    }
}

void Color::InitColorFormat(const SDL_PixelFormat* format)
{
    Color::mFormat = format;
}

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    SetRGBA(r, g, b, a);
}
void Color::SetRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    mColor = SDL_MapRGBA(mFormat, r, g, b, a);
}

void Color::SetRed(uint8_t red)
{
    uint8_t r, g, b, a;

    SDL_GetRGBA(mColor, mFormat, &r, &g, &b, &a);
    SetRGBA(red, g, b, a);
}

void Color::SetGreen(uint8_t green)
{
    uint8_t r, g, b, a;

    SDL_GetRGBA(mColor, mFormat, &r, &g, &b, &a);
    SetRGBA(r, green, b, a);
}

void Color::SetBlue(uint8_t blue)
{
    uint8_t r, g, b, a;

    SDL_GetRGBA(mColor, mFormat, &r, &g, &b, &a);
    SetRGBA(r, g, blue, a);
}

void Color::SetAlpha(uint8_t alpha)
{
    uint8_t r, g, b, a;

    SDL_GetRGBA(mColor, mFormat, &r, &g, &b, &a);
    SetRGBA(r, g, b, alpha);
}

uint8_t Color::GetRed() const
{
    uint8_t r, g, b, a;

    SDL_GetRGBA(mColor, mFormat, &r, &g, &b, &a);
    return r;
}
uint8_t Color::GetGreen() const
{
    uint8_t r, g, b, a;

    SDL_GetRGBA(mColor, mFormat, &r, &g, &b, &a);
    return g;
}
uint8_t Color::GetBlue() const
{
    uint8_t r, g, b, a;

    SDL_GetRGBA(mColor, mFormat, &r, &g, &b, &a);
    return b;
}
uint8_t Color::GetAlpha() const 
{
    uint8_t r, g, b, a;

    SDL_GetRGBA(mColor, mFormat, &r, &g, &b, &a);
    return a;
}

Color Color::Evaluate1MinusSourceAlpha(const Color &source, const Color &destination)
{
    uint8_t alpha = source.GetAlpha();
    float sourceAlpha = float(alpha) / 255.0f;
    double destAlpha = 1.0- sourceAlpha;

    Color outColor;
    
    outColor.SetAlpha(255);
    outColor.SetRed(float(source.GetRed()) * sourceAlpha + destination.GetRed() * destAlpha);
    outColor.SetGreen(float(source.GetGreen()) * sourceAlpha + destination.GetGreen() * destAlpha);
    outColor.SetBlue(float(source.GetBlue()) * sourceAlpha + destination.GetBlue() * destAlpha);

    return outColor;
}

PieceColor Color::GetRandomPieceColorEnum() {
    std::random_device rand;
    std::mt19937 gen(rand());
    std::uniform_int_distribution<> dis(1, static_cast<int>(PieceColor::YELLOW));
    
    return static_cast<PieceColor>(dis(gen));
}

SDL_Color Color::ToSDLColor() const {
    SDL_Color sdlColor;
    sdlColor.r = GetRed();
    sdlColor.g = GetGreen();
    sdlColor.b = GetBlue();
    sdlColor.a = GetAlpha();  // If you don't have an alpha value, you can set this to 255
    return sdlColor;
}

Color Color::GetColorFromId(int colorId)
{
    PieceColor enumColorId = static_cast<PieceColor>(colorId);

    switch (enumColorId)
    {
    case PieceColor::RED:
        return Color::Red();
    case PieceColor::BLUE:
        return Color::Blue();
    case PieceColor::GREEN:
        return Color::Green();
    case PieceColor::YELLOW:
        return Color::Yellow();
    default:
        return Color::DarkGrey();
    }
}
