#include "Crackit.hpp"
#include "GameScene.hpp"
#include "Game.hpp"
#include <SDL.h>
#include <cassert>
#include <SDL_ttf.h>

Crackit& Crackit::Singleton()
{
    static Crackit crackit;
    return crackit;
}

//can be optimized
bool Crackit::Init(uint32_t width, uint32_t height, uint32_t mag)
{
    mnoptrWindow = mScreen.Init(width, height, mag);

    std::unique_ptr<Game> game = std::make_unique<Game>();
    mGameScene = std::make_unique<GameScene>(std::move(game));
    mGameScene->Init();
    
    mInputController.SetGameController(GetGameScene()->GetGameController());
    SDL_SetWindowTitle(mnoptrWindow, GetGameScene()->GetSceneName().c_str());
    return mnoptrWindow != nullptr;
}

void Crackit::Run()
{
    if (mnoptrWindow)
    {
        bool running = true;

        uint32_t lastTick = SDL_GetTicks();
        uint32_t currentTick = lastTick;

        uint32_t dt = 20;
        uint32_t accumulator = 0;

        mInputController.Init([&running](uint32_t dt, InputState state) {
            running = false;
        });

        while (running)
        {
            currentTick = SDL_GetTicks();
            uint32_t frameTime = currentTick - lastTick;

            if (frameTime > 300)
            {
                frameTime = 300;
            }

            lastTick = currentTick;

            accumulator += frameTime;

            //Input
            mInputController.Update(dt);

            GameScene* gameScene = Crackit::GetGameScene();
            assert(gameScene && "Why don't we have a scene");
            
            if (gameScene)
            {
                //Update
                while (accumulator >= dt)
                {
                    //update curret scene by dt
                    gameScene->Update(dt);
                    accumulator -= dt;
                }
                //Render
                gameScene->Draw(mScreen);
            }

            mScreen.SwapScreen();
        }
    }
}

const std::string& Crackit::GetBasePath()
{
	static std::string basePath = SDL_GetBasePath();

	return basePath;
}
