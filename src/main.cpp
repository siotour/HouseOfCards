#include<iostream>
#include<SDL2/SDL.h>

using namespace std;

int main(int argc, char** argv)
{
    SDL_Window* window;
    
    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        cerr << "Unable to initialize SDL: " << SDL_GetError() << endl;
        SDL_Quit();
        return 1;
    };
    
    window = SDL_CreateWindow("House of cards",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              1200,
                              600,
                              SDL_WINDOW_OPENGL);
    
    if(window == nullptr) {
        cerr << "Unable to create window: " << SDL_GetError() << endl;
        SDL_Quit();
        return 1;
    }
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    if(renderer == nullptr) {
        cerr << "Unable to create renderer:" << SDL_GetError();
        SDL_Quit();
        return 1;
    }

    SDL_Quit();
    return 0;
}

