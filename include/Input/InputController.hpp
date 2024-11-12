#ifndef INPUT_INPUTCONTROLLER_H_
#define INPUT_INPUTCONTROLLER_H_

#include "InputAction.hpp"

class GameController;

class InputController
{
    public:
        InputController();
        void Init(InputAction quitAction);
        void Update(uint32_t dt);

        void SetGameController(GameController* controller);
    private:
        InputAction mQuit;
        GameController* mGameController;

};

#endif