#include "AARectangle.hpp"
#include "Utils.hpp"
#include <cmath>
#include <algorithm>
#include <vector>

#include <SDL.h>

AARectangle::AARectangle(): AARectangle(Vec2D::Zero, 0, 0) {}

AARectangle::AARectangle(const Vec2D& topLeft, unsigned int width, unsigned int height)
{
    mPoints.push_back(topLeft);
    mPoints.push_back(Vec2D(topLeft.GetX() + width - 1, topLeft.GetY()+height-1));
}

AARectangle::AARectangle(const Vec2D& topLeft, const Vec2D& bottomRight)
{
    mPoints.push_back(topLeft);
    mPoints.push_back(bottomRight);
}

void AARectangle::Rotate(float angle, const Vec2D &aroundPoint)
{
    if (angle == 0 || aroundPoint == GetCenterPoint()) {
        return;
    }

    Vec2D originalTopRight(mPoints[1].GetX(), mPoints[0].GetY());
    Vec2D originalBottomLeft(mPoints[0].GetX(), mPoints[1].GetY());

    // Rotate all four corners around the given point
    mPoints[0].Rotate(angle, aroundPoint); // original top-left
    mPoints[1].Rotate(angle, aroundPoint); // original bottom-right
    originalTopRight.Rotate(angle, aroundPoint);
    originalBottomLeft.Rotate(angle, aroundPoint);

    // Gather all the points
    std::vector<Vec2D> rotatedPoints = {mPoints[0], mPoints[1], originalTopRight, originalBottomLeft};

    float minX = rotatedPoints[0].GetX();
    float minY = rotatedPoints[0].GetY();
    float maxX = rotatedPoints[0].GetX();
    float maxY = rotatedPoints[0].GetY();

    for (const Vec2D& point : rotatedPoints) {
        if (point.GetX() < minX) {
            minX = point.GetX();
        }
        if (point.GetY() < minY) {
            minY = point.GetY();
        }
        if (point.GetX() > maxX) {
            maxX = point.GetX();
        }
        if (point.GetY() > maxY) {
            maxY = point.GetY();
        }
    }

    Vec2D newTopLeft(minX, minY);
    Vec2D newBottomRight(maxX, maxY);

    mPoints[0] = newTopLeft;
    mPoints[1] = newBottomRight;
}


int AARectangle::GetWidth() const
{
    return GetBottomRightPoint().GetX() - GetTopLeftPoint().GetX() + 1;
}

int AARectangle::GetHeight() const
{
    return GetBottomRightPoint().GetY() - GetTopLeftPoint().GetY() + 1;
}

void AARectangle::MoveTo(const Vec2D& position)
{
    float width = GetWidth();
    float height = GetHeight();

    SetTopLeftPoint(position);
    SetBottomRightPoint(Vec2D(position.GetX() + width - 1, position.GetY() + height -1));
}

Vec2D AARectangle::GetCenterPoint() const
{
    return Vec2D(GetTopLeftPoint().GetX() + GetWidth()/2.0f - 1, GetTopLeftPoint().GetY() + GetHeight()/2.0f - 1);
}

bool AARectangle::Intersects(const AARectangle& otherRect) const
{
    return !(otherRect.GetBottomRightPoint().GetX() < GetTopLeftPoint().GetX() || 
            otherRect.GetTopLeftPoint().GetX() > GetBottomRightPoint().GetX() ||
            otherRect.GetBottomRightPoint().GetY() < GetTopLeftPoint().GetY() ||
            otherRect.GetTopLeftPoint().GetY() > GetBottomRightPoint().GetY());
}

bool AARectangle::ContainsPoint(const Vec2D& point) const
{
    bool withinX = point.GetX() >= GetTopLeftPoint().GetX() && point.GetX() <= GetBottomRightPoint().GetX();
    bool withinY = point.GetY() >= GetTopLeftPoint().GetY() && point.GetY() <= GetBottomRightPoint().GetY();
    return withinX && withinY;
}

AARectangle AARectangle::Inset(const AARectangle& rect, Vec2D& insets)
{
    return AARectangle(rect.GetTopLeftPoint() + insets, rect.GetWidth() - 2 * insets.GetX(), rect.GetHeight() - 2*insets.GetY());
}

std::vector<Vec2D> AARectangle::GetPoints() const
{
    std::vector<Vec2D> points;

    points.push_back(mPoints[0]);
    points.push_back(Vec2D(mPoints[1].GetX(), mPoints[0].GetY()));
    points.push_back(mPoints[1]);
    points.push_back(Vec2D(mPoints[0].GetX(), mPoints[1].GetY()));

    return points;
}

void AARectangle::SetCorners(std::vector<Vec2D> points)
{
    Vec2D minVec(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());

    for (const auto& point : points) {
        if (point.GetX() < minVec.GetX() && point.GetY() < minVec.GetY()) {
            minVec = point;
        }
    }

    Vec2D newTopLeft = minVec;
    SetTopLeftPoint(newTopLeft);
    SetBottomRightPoint(Vec2D(newTopLeft.GetX() + GetWidth() - 1, newTopLeft.GetY()+GetHeight()-1));
}