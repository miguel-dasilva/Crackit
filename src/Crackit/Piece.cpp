#include "Piece.hpp"
#include "Color.hpp"
#include "Circle.hpp"
#include "AARectangle.hpp"
#include "Screen.hpp"
#include "Utils.hpp"

#include <cmath>
#include <SDL.h>


Piece::Piece(const Vec2D &pos, int cellSize): mBBox(pos * cellSize, cellSize - 1, cellSize - 1), mVelocity(Vec2D::Zero)
{
    mPieceColor = Color(Color::GetRandomPieceColorEnum());
}

void Piece::Update(uint32_t dt, Vec2D dir)
{
    mVelocity += dir;
    mBBox.MoveBy(mVelocity * MillisecondsToSeconds(dt));
}

void Piece::Draw(Screen &screen)
{
    //AARectangle rect = {mBBox.GetTopLeftPoint(), mBBox.GetBottomRightPoint()};
    Circle circ = {mBBox.GetCenterPoint(), GetRadius()};
    screen.Draw(circ, GetPieceColor(), true, GetPieceColor());
}

void Piece::RotateAroundPoint(float angle, Vec2D aroundPoint)
{
    mBBox.Rotate(angle, aroundPoint); 
}

void Piece::MoveBy(Vec2D vec)
{
    mBBox.MoveBy(vec);
}

void Piece::MoveTo(const Vec2D &newPosition)
{
    mBBox.MoveTo(newPosition);
}
