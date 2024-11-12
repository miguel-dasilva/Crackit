#include "Pair.hpp"
#include "Utils.hpp"
#include "Screen.hpp"

#include <algorithm>
#include <limits>
#include <cmath>
#include <SDL.h>

const Vec2D Pair::SECOND_PIECE_OFFSET = {0, 1};

Pair::Pair() : Pair(Vec2D::Zero, 0)
{
}

Pair::Pair(Vec2D initialPos, int cellSize)
{
    Piece firstPiece = {initialPos, cellSize};
    Piece secondPiece = {initialPos - SECOND_PIECE_OFFSET, cellSize};

    mFirstPiece = firstPiece;
    mSecondPiece = secondPiece;
    mSpeedEnhance = Vec2D::Zero;
    mBBox = {mSecondPiece.GetBoundingRect().GetTopLeftPoint(), mFirstPiece.GetBoundingRect().GetBottomRightPoint()};
}

void Pair::MoveLeft(int cellSize)
{
    mFirstPiece.MoveBy(Vec2D(-cellSize, 0));
    mSecondPiece.MoveBy(Vec2D(-cellSize, 0));
    mBBox.MoveBy(Vec2D(-cellSize, 0));
}

void Pair::MoveRight(int cellSize)
{
    mFirstPiece.MoveBy(Vec2D(cellSize, 0));
    mSecondPiece.MoveBy(Vec2D(cellSize, 0));
    mBBox.MoveBy(Vec2D(cellSize, 0));
}

Vec2D Pair::SnapPosition(const Vec2D& position, int cellSize)
{
    float snappedX = round(position.GetX() / cellSize) * cellSize;
    float snappedY = round(position.GetY() / cellSize) * cellSize;
    return Vec2D(snappedX, snappedY);
}
void Pair::SetVelocity(Vec2D speedEnhanced)
{
    mFirstPiece.SetVelocity(speedEnhanced);
    mSecondPiece.SetVelocity(speedEnhanced);
}

void Pair::RotateLeft(int cellSize)
{
    mFirstPiece.RotateAroundPoint(ROTATE_90_DEGREES, mSecondPiece.GetBoundingRect().GetCenterPoint());
}

void Pair::RotateRight()
{
    mFirstPiece.RotateAroundPoint(-ROTATE_90_DEGREES, mSecondPiece.GetBoundingRect().GetCenterPoint());

}

AARectangle Pair::RotateRightResult()
{
    AARectangle boundingRectAux = mFirstPiece.GetBoundingRect();
    boundingRectAux.Rotate(-PI/2, mSecondPiece.GetBoundingRect().GetCenterPoint());
    
    return boundingRectAux;
}

AARectangle Pair::RotateLeftResult()
{
    AARectangle boundingRectAux = mFirstPiece.GetBoundingRect();
    boundingRectAux.Rotate(PI/2, mSecondPiece.GetBoundingRect().GetCenterPoint());
    
    return boundingRectAux;
}

void Pair::Update(uint32_t dt)
{
    GetFirstPiece().Update(dt, mSpeedEnhance);
    GetSecondPiece().Update(dt, mSpeedEnhance);
}

void Pair::Draw(Screen &screen)
{
    GetFirstPiece().Draw(screen);
    GetSecondPiece().Draw(screen);
}

std::vector<Vec2D> Pair::GetAllPiecePoints() 
{
    std::vector<Vec2D> points = mFirstPiece.GetBoundingRect().GetPoints();
    std::vector<Vec2D> secondPiecePoints = mSecondPiece.GetBoundingRect().GetPoints();
    points.insert(points.end(), secondPiecePoints.begin(), secondPiecePoints.end());
    return points;
}

std::vector<Piece> Pair::SplitPieces()
{
    return std::vector<Piece>() = {mFirstPiece, mSecondPiece};
}

float Pair::GetMostLeftPoint()
{
    std::vector<Vec2D> points = GetAllPiecePoints();
    auto minElem = std::min_element(points.begin(), points.end(), 
        [](const Vec2D &a, const Vec2D &b) {
            return a.GetX() < b.GetX();
        });

    return minElem->GetX();

}


float Pair::GetMostRightPoint()
{
    std::vector<Vec2D> points = GetAllPiecePoints();

    auto maxElem = std::max_element(points.begin(), points.end(), 
        [](const Vec2D &a, const Vec2D &b) {
            return a.GetX() < b.GetX();
        });
    return maxElem->GetX();
}

float Pair::GetMostBottomPoint()
{
    std::vector<Vec2D> points = GetAllPiecePoints();
        auto maxElem = std::max_element(points.begin(), points.end(), 
        [](const Vec2D& a, const Vec2D& b) {
            return a.GetY() < b.GetY();
        });

    return maxElem->GetY();

}