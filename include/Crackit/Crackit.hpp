#ifndef CRACKIT_CRACKIT_H_
#define CRACKIT_CRACKIT_H_

#include "Screen.hpp"
#include "InputController.hpp"
#include <vector>
#include <stdint.h>
#include <memory>

struct SDL_Window;
class GameScene;

class Crackit
{
    public:
        static Crackit& Singleton();
        bool Init(uint32_t width, uint32_t height, uint32_t mag);
        void Run();

        inline uint32_t Width() const {return mScreen.Width();}
        inline uint32_t Height() const {return mScreen.Height();}
        inline GameScene* GetGameScene() const {return mGameScene.get();}

        static const std::string& GetBasePath();

    private:
    
        Screen mScreen;
        SDL_Window* mnoptrWindow;

        std::unique_ptr<GameScene> mGameScene;

        InputController mInputController;
};

#endif