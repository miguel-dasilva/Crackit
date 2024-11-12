#include "GameScene.hpp"
#include "Screen.hpp"
#include "AARectangle.hpp"
#include "Circle.hpp"
#include "Color.hpp"
#include "GameController.hpp"
#include <SDL.h>
#include <SDL_ttf.h>
#include "Crackit.hpp"

//This class can be used to setup a game options / main menu screen later
GameScene::GameScene(std::unique_ptr<Game> optrGame):mGame(std::move(optrGame))
{
}

void GameScene::Init()
{
    mGame->Init(mGameController);
}

void GameScene::Update(uint32_t dt)
{
    mGame->Update(dt);
}

void GameScene::Draw(Screen &theScreen)
{
    mGame->Draw(theScreen);
}

const std::string &GameScene::GetSceneName()
{
    static std::string sceneName = "Crackit";
    return sceneName;
    // TODO: insert return statement here
}
