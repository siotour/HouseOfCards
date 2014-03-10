#include"../include/sdlutility.hpp"
#include<avl/include/exceptions.hpp>
#include<SDL2/SDL.h>
#include<string>

using namespace std;
using namespace avl;


SDLContext::SDLContext(const string& windowTitle, const unsigned int screenWidth, const unsigned int screenHeight, const bool fullscreen)
    : title(windowTitle),
    width(screenWidth),
    height(screenHeight),
    fullscreen(fullscreen)
{
    window = createWindow();
    renderer = createRenderer();
}

SDLContext::~SDLContext() {
    cleanup();
}

unsigned int SDLContext::getWidth() const {
    return width;
}

unsigned int SDLContext::getHeight() const {
    return height;
}

bool SDLContext::isFullscreen() const {
    return fullscreen;
}

SDL_Window* SDLContext::getWindow() {
    return window;
}

SDL_Renderer* SDLContext::getRenderer() {
    return renderer;
}

void SDLContext::initSDL() {
    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        cleanup();
        throw SDLException(__FILE__, __LINE__, "SDL_Init()", SDL_GetError());
    };
}

void SDLContext::cleanup() {
    if(renderer != nullptr) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    
    if(window != nullptr) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    
    SDL_Quit();
}

SDL_Window* SDLContext::createWindow() {
    int flags = SDL_WINDOW_OPENGL;
    if(fullscreen == true) {
        flags |= SDL_WINDOW_FULLSCREEN;
    }
    SDL_Window* tempWindow = SDL_CreateWindow(title.c_str(),
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              width,
                              height,
                              static_cast<SDL_WindowFlags>(flags));
    
    if(tempWindow == nullptr) {
        cleanup();
        throw SDLException(__FILE__, __LINE__, "SDL_CreateWindow()", SDL_GetError());
    }
    
    return tempWindow;
}

SDL_Renderer* SDLContext::createRenderer() {
//    avlAssert(window != nullptr);
    
    SDL_Renderer* tempRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    if(tempRenderer == nullptr) {
        cleanup();
        throw SDLException(__FILE__, __LINE__, "SDL_CreateRenderer()", SDL_GetError());
    }
    
    return tempRenderer;
}


SDLException::SDLException(const std::string& fileName, const unsigned int lineNum, const std::string& funcName, const std::string& errorDesc)
    : Exception(fileName, lineNum),
    function(funcName),
    error(errorDesc)
{
}

const std::string& SDLException::getFunction() const {
    return function;
}

const std::string& SDLException::getError() const {
    return error;
}