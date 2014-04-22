#include"../include/sdlutility.hpp"
#include<avl/include/utility.hpp>
#include<avl/include/exceptions.hpp>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<string>

using namespace std;
using namespace avl;


SDL_Texture* loadTexture(SDL_Renderer* const renderer, const std::string& filePath) {
    if(renderer == nullptr) {
        throw NullPointerException(__FILE__, __LINE__);
    }
    SDL_Texture* texture = IMG_LoadTexture(renderer, filePath.c_str());
    if(texture == nullptr) {
        throw SDLException(__FILE__, __LINE__, "IMG_LoadTexture()", IMG_GetError());
    }
    return texture;
}

void unloadTexture(SDL_Texture* const texture) {
    SDL_DestroyTexture(texture);
}

const SDL_Rect toSDL_Rect(const avl::AABB2<int>& original) {
    SDL_Rect rect;
    rect.x = original.left;
    rect.y = original.top;
    rect.w = original.getWidth();
    rect.h = original.getHeight();
    
    return rect;
}


int SDL_RenderCopy(SDL_Renderer* renderer, SDL_Texture* texture, const avl::AABB2<int>* srcAABB, const avl::AABB2<int>* dstAABB) {
    SDL_Rect srcRect = toSDL_Rect(*srcAABB);
    SDL_Rect dstRect = toSDL_Rect(*dstAABB);
    return SDL_RenderCopy(renderer, texture, &(srcRect), &(dstRect));
}


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
    // This implies that only one context can exist at a time.
    SDL_Quit();
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



SDLTextureLoader::SDLTextureLoader(SDLContext& context)
: context(context)
{
}

SDL_Texture* SDLTextureLoader::load(const std::string& filePath) {
    return loadTexture(context.getRenderer(), filePath);
}

void SDLTextureLoader::unload(SDL_Texture* const texture) {
    unloadTexture(texture);
}