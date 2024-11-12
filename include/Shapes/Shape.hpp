#ifndef SHAPES_SHAPE_H_
#define SHAPES_SHAPE_H_

#include "Vec2D.hpp"
#include <vector>

class Shape
{
    public:
        virtual ~Shape() {}
        virtual Vec2D GetCenterPoint() const = 0;
        virtual void MoveTo(const Vec2D& p) = 0;
        inline virtual std::vector<Vec2D> GetPoints() const {return mPoints;}
        void MoveBy(const Vec2D& deltaOffset);
    protected:
        std::vector<Vec2D> mPoints;
};

#endif