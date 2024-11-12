#ifndef GAMEGRID_H_
#define GAMEGRID_H_

#include "Piece.hpp"

class Screen;
class AARectangle;

class GameGrid {
    public:
        static constexpr int NUM_COLS = 8;
        static constexpr int NUM_ROWS = 16;
        static constexpr int CELLSIZE = 40;

        GameGrid();
        int grid[NUM_ROWS][NUM_COLS]; //[rows][columns]
        
        void Init();
        void Update(uint32_t dt);
        void Draw(Screen& screen);
        void AddLockedPiece(Piece piece);

        inline int GetCellSize() { return cellSize; }
        inline int GetNumColls() { return mNumCols; }
        bool IsCellAvailable(Vec2D pos);
        bool IsPieceColliding(Piece piece, Vec2D dir = Vec2D::Zero) const;
        Vec2D ScreenPosToGridPos(const Vec2D& screenPos) const;
        Vec2D GridPosToScreenPos(const Vec2D& gridPos) const;
        bool CheckForGroups(int& multiplier);
        void DropCells();
    
    private:
        int mNumRows;
        int mNumCols;
        int cellSize;

        bool IsWithinBounds(const Vec2D& gridPos) const;
        std::vector<Piece> mLockedPieces;

        void DropCellsInColumn(int col);
        int floodFill(int r, int c, int color, std::vector<std::pair<int, int>>& cellsToClear);
        
        static const int DIRECTIONS = 4;
        static const int MINIMUM_GROUP = 4;
        int dr[DIRECTIONS] = {-1, 1, 0, 0};
        int dc[DIRECTIONS] = {0, 0, -1, 1};
        std::vector<std::vector<bool>> visited;

};
#endif