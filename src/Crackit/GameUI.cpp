#include "GameUI.hpp"
#include "AARectangle.hpp"
#include "Screen.hpp"
#include "SDL.h"
#include <chrono>

GameUI::GameUI() : GameUI(Vec2D::Zero)
{
}

GameUI::GameUI(const Vec2D &position) : mPosition(position), mCurrentLevel(1), mGoalScore(200), mCurrentScore(0)
{
}

void GameUI::Init(double countDownTimer)
{
    lastTime = std::chrono::high_resolution_clock::now();
    mCountdownTimer = countDownTimer;
}

void GameUI::Update(uint32_t dt)
{
    auto currentTime = std::chrono::high_resolution_clock::now();
    double elapsedTime = std::chrono::duration<double>(currentTime - lastTime).count();

    mCountdownTimer -= elapsedTime;

    lastTime = currentTime;
}

void GameUI::DrawGameOver(Screen& screen, int gridWidth)
{
    int screenWidth, screenHeight, gameOverWidth, gameOverHeight, restartWidth, restartHeight;
    SDL_GetWindowSize(screen.GetWindow(), &screenWidth, &screenHeight);

    int uiMidPointX = gridWidth + (screenWidth - gridWidth) / 2;
    int uiMidPointY = screenHeight / 2;

    screen.GetTextDimensions(GAME_OVER_MESSAGE, gameOverWidth, gameOverHeight, BIG_FONT);
    screen.GetTextDimensions(RESTART_MESSAGE, restartWidth, restartHeight, MEDIUM_FONT);

    screen.Draw(GAME_OVER_MESSAGE, Vec2D(uiMidPointX, uiMidPointY) - Vec2D(gameOverWidth / 2, gameOverHeight / 2), Color::White(), BIG_FONT);
    screen.Draw(RESTART_MESSAGE, Vec2D(uiMidPointX, uiMidPointY + RESTART_POS_OFFSET) - Vec2D(restartWidth / 2, restartHeight / 2), Color::White(), MEDIUM_FONT);
}
void GameUI::Draw(Screen &screen, int gridWidth)
{
    int screenWidth, screenHeight, titleHeight, titleWidth;
    SDL_GetWindowSize(screen.GetWindow(), &screenWidth, &screenHeight);

    AARectangle UIRectangle = AARectangle(mPosition, screenWidth / 2, screenHeight);

    int uiMidPointX = gridWidth + (screenWidth - gridWidth) / 2;
    int uiMidPointY = screenHeight / 2;

    Vec2D titlePos = mPosition;
    Vec2D levelPos = mPosition + Vec2D(0, 50);
    Vec2D timerPos = mPosition + Vec2D(0, 100);
    Vec2D goalPos = mPosition + Vec2D(0, 150);

    screen.Draw(UIRectangle, Color::White());
    screen.Draw("Crackit", titlePos, Color::White(), BIG_FONT);
    screen.Draw("Level: " + std::to_string(mCurrentLevel), levelPos, Color::White(), MEDIUM_FONT);
    screen.Draw("Time: " + std::to_string(mCountdownTimer), timerPos, Color::White(), MEDIUM_FONT);
    screen.Draw("Goal: " + std::to_string(mGoalScore) + " Current: " + std::to_string(mCurrentScore), goalPos, Color::White(), MEDIUM_FONT);
}
