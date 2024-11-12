#ifndef PAIR_H_
#define PAIR_H_

#include "Piece.hpp"
#include <vector>

class Vec2D;
class Screen;

static const Vec2D LEFT_DIR = {-1, 0};
static const Vec2D RIGHT_DIR = {1, 0};

class Pair {
    public:
        Pair();
        Pair(Vec2D initialPos, int cellSize);

        void MoveLeft(int cellSize);
        void MoveRight(int cellSize);

        void RotateLeft(int cellSize);
        void RotateRight();
        Vec2D SnapPosition(const Vec2D& position, int cellSize);

        void Update(uint32_t dt);
        void Draw(Screen& screen);
        void SetVelocity(Vec2D speedEnhanced);

        AARectangle RotateRightResult();
        AARectangle RotateLeftResult();

        inline Piece& GetFirstPiece() { return mFirstPiece; }
        inline Piece& GetSecondPiece() { return mSecondPiece; }

        inline Vec2D GetTopLeftPoint() { return mBBox.GetTopLeftPoint();}
        inline Vec2D GetBottomRightPoint() { return mBBox.GetBottomRightPoint();}
        
        float GetMostLeftPoint();
        float GetMostRightPoint();
        float GetMostBottomPoint();
        std::vector<Vec2D> GetAllPiecePoints();
        std::vector<Piece> SplitPieces();
        
    private:
        AARectangle mBBox;
        Vec2D mSpeedEnhance;
        Vec2D mPos;
        uint32_t mDirection;
        Piece mFirstPiece;
        Piece mSecondPiece;

        static constexpr float PI = 3.1415;
        static constexpr float ROTATE_90_DEGREES = PI/2;
        static const Vec2D SECOND_PIECE_OFFSET;
};

#endif