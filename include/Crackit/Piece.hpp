#ifndef PIECE_H_
#define PIECE_H_

#include "Color.hpp"
#include "AARectangle.hpp"

#include <SDL.h>
class Screen;

class Piece
{
    public:
        Piece() = default;
        Piece(const Vec2D& pos, int cellSize);

        void Update(uint32_t dt, Vec2D dir = Vec2D::Zero);
        void Draw(Screen& screen);

        void RotateAroundPoint(float angle, Vec2D aroundPoint);
        inline const Vec2D GetPiecePos() {return mBBox.GetCenterPoint();}
        inline Color GetPieceColor() {return mPieceColor;}
        inline Vec2D& GetVelocity() {return mVelocity;}
        inline void SetVelocity(const Vec2D& vel) {mVelocity = vel;}
        inline float GetRadius() const {return mBBox.GetWidth()/2.0f;}
        inline const AARectangle& GetBoundingRect() const {return mBBox;}
        void MoveBy(Vec2D vec);
        void MoveTo(const Vec2D& newCenterPosition);
    private:
        AARectangle mBBox;
        Vec2D mPos;
        Vec2D mVelocity;
        Color mPieceColor;


        static const float RADIUS;

};

#endif