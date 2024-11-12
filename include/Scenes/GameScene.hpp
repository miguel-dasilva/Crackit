#ifndef SCENES_GAMESCENE_H_
#define SCENES_GAMESCENE_H_

#include "GameController.hpp"
#include "Game.hpp"
#include <memory>

class GameScene
{
    public:
        GameScene(std::unique_ptr<Game> optrGame);
        void Init();
        void Update(uint32_t dt);
        void Draw(Screen& theScreen);
        const std::string& GetSceneName();
        GameController* GetGameController() {return &mGameController;}

    private:
        GameController mGameController;
        std::unique_ptr<Game> mGame;
};
#endif