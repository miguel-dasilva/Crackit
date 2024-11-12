#include "GameGrid.hpp"
#include "AARectangle.hpp"
#include "Circle.hpp"
#include "Screen.hpp"
#include "Color.hpp"
#include "Vec2D.hpp"

#include <SDL.h>
#include <cmath>
#include <cassert>

GameGrid::GameGrid() : mNumCols(NUM_COLS), mNumRows(NUM_ROWS), cellSize(CELLSIZE)
{
    visited.resize(mNumRows, std::vector<bool>(mNumCols, false));
}

void GameGrid::Init()
{
    for (int row = 0; row < mNumRows; row++)
    {
        for(int column = 0; column < mNumCols; column++)
        {
            grid[row][column] = 0;
        }
    }
}

void GameGrid::Update(uint32_t dt)
{

}

void GameGrid::Draw(Screen& screen)
{
    for (int row = 0; row < mNumRows; row++) 
    {
        for (int col = 0; col < mNumCols; col++)
        {
            int cellValue = grid[row][col];
            if (cellValue != 0)
            {
                Circle circ = { Vec2D(col * cellSize + static_cast<float>(cellSize) / 2.0f, row * cellSize + static_cast<float>(cellSize) / 2.0f), static_cast<float>(cellSize) / 2.0f};
                screen.Draw(circ, Color::GetColorFromId(cellValue), true, Color::GetColorFromId(cellValue));
            }
        }
    }
}

int GameGrid::floodFill(int x, int y, int currentColor, std::vector<std::pair<int, int>>& cellsToClear)
{
    if (!IsWithinBounds(Vec2D(y, x)) || visited[x][y] || grid[x][y] != currentColor) return 0;

    visited[x][y] = true;
    cellsToClear.push_back({x, y});

    int size = 1;

    size += floodFill(x-1, y, currentColor, cellsToClear);
    size += floodFill(x+1, y, currentColor, cellsToClear);
    size += floodFill(x, y-1, currentColor, cellsToClear);
    size += floodFill(x, y+1, currentColor, cellsToClear);


    return size;
}

//Algorithm to clear the groups using a flood fill approach
bool GameGrid::CheckForGroups(int& multiplier) {

    for (int i = 0; i < mNumRows; i++)
        std::fill(visited[i].begin(), visited[i].end(), false);
    
    bool foundGroup = false;
    for (int i = 0; i < mNumRows; i++) {
        for (int j = 0; j < mNumCols; j++) {
            if (grid[i][j] > 0 && !visited[i][j]) {
                std::vector<std::pair<int, int>> cellsToClear;
                int groupSize = floodFill(i, j, grid[i][j], cellsToClear);
                if (groupSize >= MINIMUM_GROUP) {

                    foundGroup = true;
                    multiplier++;

                    for (const auto& cell : cellsToClear) {
                        grid[cell.first][cell.second] = 0;
                    }

                }
            }
        }
    }
    return foundGroup;
}

// Converts from a screen position to grid coordinates
Vec2D GameGrid::ScreenPosToGridPos(const Vec2D& screenPos) const
{
    float col = std::round(screenPos.GetX() / cellSize);
    float row = std::round(screenPos.GetY() / cellSize);
    return Vec2D(col, row);
}

// Converts from grid coordinates to a screen position
Vec2D GameGrid::GridPosToScreenPos(const Vec2D& gridPos) const
{
    float x = gridPos.GetX() * cellSize;
    float y = gridPos.GetY() * cellSize;
    return Vec2D(x, y);
}


bool GameGrid::IsPieceColliding(Piece piece, Vec2D dir) const
{
    Vec2D gridPos = ScreenPosToGridPos(piece.GetBoundingRect().GetTopLeftPoint());
    gridPos += dir;
    int row = static_cast<int>(gridPos.GetY());
    int col = static_cast<int>(gridPos.GetX());
    if(grid[row][col] != 0) {
        return true;
    } 
    else {
        return false;
    }
}
bool GameGrid::IsWithinBounds(const Vec2D& gridPos) const
{
    return (gridPos.GetX() >= 0 && gridPos.GetX() < mNumCols && 
           gridPos.GetY() >= 0 && gridPos.GetY() < mNumRows);
}

bool GameGrid::IsCellAvailable(Vec2D gridPos)
{
    int row = static_cast<int>(gridPos.GetY());
    int col = static_cast<int>(gridPos.GetX());
    return IsWithinBounds(gridPos) && grid[row][col] == 0;
}

void GameGrid::AddLockedPiece(Piece piece)
{
    Vec2D gridPos = ScreenPosToGridPos(piece.GetBoundingRect().GetCenterPoint());
    int row = static_cast<int>(gridPos.GetY());
    int col = static_cast<int>(gridPos.GetX());

    assert(row < mNumRows);
    assert(col < mNumCols);
    grid[row][col] = piece.GetPieceColor().GetColorId();
}

void GameGrid::DropCells()
{
    for (int col = 0; col < mNumCols; col++)
    {
        DropCellsInColumn(col);
    }
}

void GameGrid::DropCellsInColumn(int col) {
    bool cellMovedInThisColumn;
    do {
        cellMovedInThisColumn = false;
        for (int row = mNumRows - 2; row >= 0; row--) {
            if (grid[row][col] != 0 && grid[row + 1][col] == 0) {
                grid[row + 1][col] = grid[row][col];
                grid[row][col] = 0;
                cellMovedInThisColumn = true;
            }
        }
    } while (cellMovedInThisColumn);
}