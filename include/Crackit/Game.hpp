#ifndef GAME_GAME_HPP
#define GAME_GAME_HPP

#include "GameUI.hpp"
#include "GameGrid.hpp"
#include "Pair.hpp"

#include <string>
#include <memory>

class GameController;
class Screen;

enum GameState
{
    SPAWNING_PAIR,
    PLAYING_PAIR,
    CHECKING_GROUPS,
    GAME_OVER
};

class Game
{
    public:
        void Init(GameController& controller);
        void Update(uint32_t dt);
        void Draw(Screen& screen);

    private:
        GameUI mGameUI;
        GameGrid mGameGrid;
        GameState mGameState;
        std::unique_ptr<Pair> mCurrentPair;

        int mCurrentScore; 
        int mMultiplier;
        int mCurrentLevel;
        
        void HandleGameOver();
        void SetupInputActions(GameController& controller);
        void Reset();
        void HandleSpawningPairState();
        void HandlePlayingPairState(uint32_t dt);
        Vec2D GetBottomGridPosition();
        bool IsPairMoveableDown(const Vec2D& bottomGridPos);
        void HandleCheckingGroupsState();
        inline void ResetMultiplier() {mMultiplier = 0;}
        inline void SetLevel(int level) {mCurrentLevel = level;}
        void UpdateScore();
        void SetNextLevel();
        void ResetGameAtLevel(int level);
        static constexpr float PIECE_RADIUS = 10;
        static constexpr float DROP_SPEED = 100;
        static constexpr float DROP_SPEED_ENHANCED = 200;
        static constexpr int COUNTDOWN_TIMER = 120;
        static constexpr int GAME_OVER_ROW = 0;
        static constexpr int GOAL_MULTIPLIER = 100;
        static constexpr int SCORE_EARN_BASE = 20;
        static const Vec2D SPAWN_POINT;
        static const Vec2D DROP_VELOCITY;

        void LockCurrentPair();
        void AddLockedPiece(Piece piece);
};

#endif