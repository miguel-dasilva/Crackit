#include <iostream>
#include "Crackit.hpp"
#include <SDL.h>

const int SCREEN_WIDTH = 720;
const int SCREEN_HEIGHT = 640;
const int MAGNIFICATION = 1;

using namespace std;

int main(int argc, char* argv[]) 
{
    if (Crackit::Singleton().Init(SCREEN_WIDTH, SCREEN_HEIGHT, MAGNIFICATION))
    {
        Crackit::Singleton().Run();
    }


    return 0;
}

