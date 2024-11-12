#ifndef GAMEUI_HPP
#define GAMEUI_HPP

#include "Vec2D.hpp"
#include <chrono>

class Screen;

class GameUI
{
    public:
        GameUI();
        GameUI(const Vec2D& position);

        void Init(double countDownTimer);
        void Update(uint32_t dt);
        void Draw(Screen &screen, int gridWidth);
        void DrawGameOver(Screen& screen, int gridWidth);
        
        inline int GetCurrentTime() {return mCountdownTimer;}
        inline void SetTimer(double timer) { mCountdownTimer = timer;}
        inline void SetGoalScore(int score) {mGoalScore = score;}
        inline void SetCurrentScore(int score) {mCurrentScore = score;}
        inline void SetLevel(int level) {mCurrentLevel = level;}
        inline int GetGoalScore() {return mGoalScore;}
        
    private:
        Vec2D mPosition;
        int mCurrentLevel;
        double mCountdownTimer;
        int mGoalScore;
        int mCurrentScore;
        
        static constexpr int BIG_FONT = 24;
        static constexpr int MEDIUM_FONT = 16;
        static constexpr int SMALL_FONT = 8;

        static constexpr int RESTART_POS_OFFSET = 100;

        inline static const std::string GAME_OVER_MESSAGE = "GameOver :(";
        inline static const std::string RESTART_MESSAGE = "Press K to restart";
        inline static const std::string TITLE_MESSAGE = "CRACKIT";
        

        std::chrono::high_resolution_clock::time_point lastTime;

};

#endif