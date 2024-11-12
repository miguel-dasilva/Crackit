#include "InputController.hpp"
#include <SDL.h>
#include "GameController.hpp"

InputController::InputController(): mQuit(nullptr), mGameController(nullptr)
{

}

void InputController::Init(InputAction quitAction)
{
    mQuit = quitAction;
}

void InputController::Update(uint32_t dt)
{
    SDL_Event sdlEvent;

    while(SDL_PollEvent(&sdlEvent))
    {
        switch (sdlEvent.type)
        {
            case SDL_QUIT:
            {
                mQuit(dt, SDL_PRESSED);
            }
            case SDL_MOUSEMOTION:
            {
                if (mGameController)
                {
                    if (MouseMovedAction mouseMoved = mGameController->GetMouseMovedAction())
                    {
                        MousePosition position;
                        position.xPos = sdlEvent.motion.x;
                        position.yPos = sdlEvent.motion.y;
                        mouseMoved(position);
                    }
                }
                break;
            }
            case SDL_MOUSEBUTTONUP:
            case SDL_MOUSEBUTTONDOWN:
                if (mGameController)
                {
                    MouseInputAction action = mGameController->GetMouseButtonActionForMouseButton(static_cast<MouseButton>(sdlEvent.button.button));
                    MousePosition position;

                    position.xPos = sdlEvent.button.x;
                    position.yPos = sdlEvent.button.y;

                    action(static_cast<InputState>(sdlEvent.button.state), position);
                }
                break;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                if (mGameController)
                {
                    InputAction action = mGameController->GetActionForKey(sdlEvent.key.keysym.sym);

                    action(dt, static_cast<InputState>(sdlEvent.key.state));
                }
                break;
        }
    }
}

void InputController::SetGameController(GameController* controller)
{
    mGameController = controller;
}