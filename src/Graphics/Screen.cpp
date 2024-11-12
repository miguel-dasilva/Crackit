#include "Screen.hpp"
#include "Vec2D.hpp"
#include "Line2D.hpp"
#include "AARectangle.hpp"
#include "Circle.hpp"
#include "Utils.hpp"
#include "Crackit.hpp"
#include <SDL.h>
#include <cmath>
#include <cassert>
#include <algorithm>

Screen::Screen(): mWidth(0), mHeight(0), moptrWindow(nullptr), mnoptrWindowSurface(nullptr)
{

}

Screen::~Screen()
{
    if (moptrWindow)
    {
        SDL_DestroyWindow(moptrWindow);
        moptrWindow = nullptr;
    }
    
    for (auto& fontPair : mFontMap)
    {
        TTF_CloseFont(fontPair.second);
    }
    TTF_Quit();
    SDL_Quit();
}

SDL_Window* Screen::Init(uint32_t w, uint32_t h, uint32_t mag)
{
    if (SDL_Init(SDL_INIT_VIDEO))
    {
        std::cout << "Error SDL_Init Failed" << std::endl;
        return nullptr;
    }

    if (TTF_Init() == -1)
    {
        SDL_Log("SDL_ttf could not initialize! SDL_ttf Error: %s", TTF_GetError());
        return nullptr;
    }

    mWidth = w;
    mHeight = h;

    moptrWindow = SDL_CreateWindow("Crackit", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        mWidth * mag, mHeight * mag, 0);

    if (moptrWindow)
    {
        mnoptrWindowSurface = SDL_GetWindowSurface(moptrWindow);
        
        SDL_PixelFormat* pixelFormat = mnoptrWindowSurface->format;

        Color::InitColorFormat(pixelFormat);

        mClearColor = Color::Black();

        mBackBuffer.Init(pixelFormat->format, mWidth, mHeight);

        mBackBuffer.Clear(mClearColor);
    }


    return moptrWindow;
}

void Screen::SwapScreen()
{
    assert(moptrWindow);

    if(moptrWindow)
    {
        ClearScreen();

        SDL_BlitScaled(mBackBuffer.GetSurface(), nullptr, mnoptrWindowSurface, nullptr);

        SDL_UpdateWindowSurface(moptrWindow);

        mBackBuffer.Clear(mClearColor);
    }
}


void Screen::Draw(int x, int y, const Color &color)
{
    assert(moptrWindow);
    if (moptrWindow)
        mBackBuffer.SetPixel(color, x, y);
}
void Screen::Draw(const Vec2D& point, const Color& color)
{
    assert(moptrWindow);
    if (moptrWindow)
        mBackBuffer.SetPixel(color, point.GetX(), point.GetY());
}

void Screen::Draw(const Line2D& line, const Color &color)
{
    assert(moptrWindow);
    if(moptrWindow)
    {
        int dx, dy;

        int x0 = roundf(line.GetP0().GetX());
        int y0 = roundf(line.GetP0().GetY());
        int x1 = roundf(line.GetP1().GetX());
        int y1 = roundf(line.GetP1().GetY());

        dx = x1 - x0;
        dy = y1 - y0;

        signed const char ix((dx > 0) - (dx < 0));
        signed const char iy((dy > 0) - (dy < 0));

        dx = abs(dx) * 2;
        dy = abs(dy) * 2;

        Draw(x0, y0, color);

        if (dx >= dy)
        {
            int d = dy - dx/2;

            while(x0 != x1)
            {
                if (d >= 0)
                {
                    d-= dx;
                    y0 += iy;
                }

                d += dy;
                x0 += ix;

                Draw(x0, y0, color);
            }
        }
        else
        {
            int d = dx - dy/2;
            while (y0 != y1)
            {
                if (d>=0)
                {
                    d -= dy;
                    x0 += ix;
                }

                d += dx;
                y0 += iy;

                Draw(x0, y0, color);
            }
        }
    }
}

void Screen::Draw(const AARectangle& rectangle, const Color& color, bool fill, const Color& fillColor)
{
    if (fill)
    {
        FillPoly(rectangle.GetPoints(), fillColor);
    }
    std::vector<Vec2D> points = rectangle.GetPoints();
    std::vector<Line2D> lines;
    
    lines.push_back(Line2D(points[0], points[1]));
    lines.push_back(Line2D(points[1], points[2]));
    lines.push_back(Line2D(points[2], points[3]));
    lines.push_back(Line2D(points[3], points[0]));

    for (Line2D line : lines)
    {
        Draw(line, color);
    }
}

void Screen::Draw(const Circle& circle, const Color& color, bool fill, const Color& fillColor)
{
    static unsigned int NUM_CIRCLE_SEGMENTS = 30;

    std::vector<Vec2D> circlePoints;
    std::vector<Line2D> lines;

    float angle = TWO_PI / float(NUM_CIRCLE_SEGMENTS);

    Vec2D p0 = Vec2D(circle.GetCenterPoint().GetX() + circle.GetRadius(), circle.GetCenterPoint().GetY());
    Vec2D p1 = p0;
    Line2D nextLineToDraw;

    for (unsigned int i = 0; i < NUM_CIRCLE_SEGMENTS; ++i)
    {
        p1.Rotate(angle, circle.GetCenterPoint());
        nextLineToDraw.SetP1(p1);
        nextLineToDraw.SetP0(p0);

        lines.push_back(nextLineToDraw);
        p0 = p1;
        circlePoints.push_back(p0);
    }

    if (fill)
    {
        FillPoly(circlePoints, fillColor);
    }

    for (const Line2D& line : lines)
    {
        Draw(line, color);
    }
}

void Screen::Draw(const std::string& text, const Vec2D& position, const Color& color, int fontSize)
{
    TTF_Font* font = GetFontOfSize(fontSize);
    if (!font) return;  // Ensure the font is available
    SDL_Color sdlColor = color.ToSDLColor();
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), sdlColor);
    
    // Use your ScreenBuffer to draw this surface on the screen
    mBackBuffer.AddSurface(textSurface, position.GetX(), position.GetY());
    
    SDL_FreeSurface(textSurface);
}

void Screen::GetTextDimensions(std::string text, int &width, int &height, int fontSize)
{
    TTF_Font* font = GetFontOfSize(fontSize);
    if (!font) return;  // Ensure the font is available

    TTF_SizeText(font, text.c_str(), &width, &height);
}


void Screen::ClearScreen()
{
    assert(moptrWindow);

    if (moptrWindow)
        SDL_FillRect(mnoptrWindowSurface, nullptr, mClearColor.GetPixelColor());
}

void Screen::FillPoly(const std::vector<Vec2D> &points, const Color &color)
{
    if (points.size() > 0)
    {
        float top = points[0].GetY();
        float bottom = points[0].GetY();
        float right = points[0].GetX();
        float left = points[0].GetX();

        for (size_t i = 1; i < points.size(); ++i)
        {
            if (points[i].GetY() < top)
            {
                top = points[i].GetY();
            }

            if(points[i].GetY() > bottom)
            {
                bottom = points[i].GetY();
            }

            if(points[i].GetX() < left)
            {
                left = points[i].GetX();
            }

            if(points[i].GetX() > right)
            {
                right = points[i].GetX();
            }
        }
        for (int pixelY = top; pixelY < bottom; ++pixelY)
        {
            std::vector<float> nodeXVec;

            size_t j = points.size() - 1;

            for (size_t i = 0; i < points.size(); ++i)
            {
                float pointiY = points[i].GetY();
                float pointjY = points[j].GetY();

                if ((pointiY <=(float)pixelY && pointjY > (float)pixelY || 
                    (pointjY <= (float)pixelY && pointiY > (float)pixelY)))
                {
                    float denom = pointjY - pointiY;
                    if (IsEqual(denom, 0))
                    {
                        continue;
                    }

                    float x = points[i].GetX() + (pixelY - pointiY)/(denom) * (points[j].GetX() - points[i].GetX());
                    nodeXVec.push_back(x);
                }

                j = i;
            }
            std::sort(nodeXVec.begin(), nodeXVec.end(), std::less<>());

            for (size_t k = 0; k < nodeXVec.size(); k += 2)
            {
                if (nodeXVec[k] > right)
                {
                    break;
                }

                if (nodeXVec[k+1] > left)
                {
                    if (nodeXVec[k] < left)
                    {
                        nodeXVec[k] = left;
                    }
                    if (nodeXVec[k+1] > right)
                    {
                        nodeXVec[k+1] = right;
                    }

                    for (int pixelX = nodeXVec[k]; pixelX < nodeXVec[k+1]; ++pixelX)
                    {
                        Draw(pixelX, pixelY, color);
                    }
                }
            }
        }

    }

}

TTF_Font *Screen::GetFontOfSize(int size)
{
    // If the font size is already loaded, return it
    if (mFontMap.find(size) != mFontMap.end())
    {
        return mFontMap[size];
    }
    // Otherwise, load the font of the given size and store it
    std::string fontPath = Crackit::Singleton().GetBasePath() + "Assets\\PressStart2P-Regular.ttf";
    TTF_Font* newFont = TTF_OpenFont(fontPath.c_str(), size);

    if (newFont)
    {
        mFontMap[size] = newFont;
    }
    return newFont;
}
