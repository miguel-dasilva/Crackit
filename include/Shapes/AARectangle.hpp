#ifndef SHAPES_AARECTANGLE_H_
#define SHAPES_AARECTANGLE_H_

#include "Shape.hpp"

class AARectangle : public Shape
{
    public:
        AARectangle();
        AARectangle(const Vec2D& topLeft, unsigned int width, unsigned int height);
        AARectangle(const Vec2D& topLeft, const Vec2D& bottomRight);

        inline void SetTopLeftPoint(const Vec2D& topLeft) {mPoints[0] = topLeft;}
        inline void SetBottomRightPoint(const Vec2D& bottomRight) {mPoints[1] = bottomRight;}

        inline Vec2D GetTopLeftPoint() const {return mPoints[0];}
        inline Vec2D GetBottomRightPoint() const {return mPoints[1];}
        
        void SetCorners(std::vector<Vec2D> points);

        void Rotate(float angle, const Vec2D& aroundPoint);
        int GetWidth() const;
        int GetHeight() const;

        virtual void MoveTo(const Vec2D& position) override;

        virtual Vec2D GetCenterPoint() const override;

        bool Intersects(const AARectangle& otherRect) const;
        bool ContainsPoint(const Vec2D& point) const;

        static AARectangle Inset(const AARectangle& rect, Vec2D& insets);

        virtual std::vector<Vec2D> GetPoints() const override;

    private:
        int mWidth, mHeight;
};

#endif