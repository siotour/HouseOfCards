#include"../include/sdlutility.hpp"
#include<avl/include/utility.hpp>
#include<avl/include/exceptions.hpp>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_mixer.h>
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

Mix_Chunk* loadSound(const string& filePath) {
    Mix_Chunk* sound = Mix_LoadWAV(filePath.c_str());
    
    if(sound == nullptr) {
        throw SDLException(__FILE__, __LINE__, "Mix_LoadWAV()", Mix_GetError());
    }
    
    return sound;
}

void unloadSound(Mix_Chunk* const sound) {
    Mix_FreeChunk(sound);
}

Mix_Music* loadMusic(const string& filePath) {
    Mix_Music* music = Mix_LoadMUS(filePath.c_str());
    
    if(music == nullptr) {
        throw SDLException(__FILE__, __LINE__, "Mix_LoadWAV()", Mix_GetError());
    }
    
    return music;
}

void unloadMusic(Mix_Music* const music) {
    Mix_FreeMusic(music);
}

const SDL_Rect toSDL_Rect(const avl::AABB2<int>& original) {
    SDL_Rect rect;
    rect.x = original.left;
    rect.y = original.top;
    rect.w = original.getWidth();
    rect.h = original.getHeight();
    
    return rect;
}


int RenderCopy(SDLContext& context, SDL_Texture* const texture, const AABB2<double>* const srcRect, const AABB2<double>* const dstRect) {
    AABB2<int>* pIntSrcRect = nullptr;
    AABB2<int> intSrcRect;
    if(srcRect != nullptr) {
        int width;
        int height;
        SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
        intSrcRect.left = srcRect->left * width;
        intSrcRect.right = srcRect->right * width;
        intSrcRect.top = srcRect->top * height;
        intSrcRect.bottom = srcRect->bottom * height;
        pIntSrcRect = &intSrcRect;
    }
    
    AABB2<int>* pIntDstRect = nullptr;
    AABB2<int> intDstRect;
    if(dstRect != nullptr) {
        intDstRect.left = dstRect->left * context.getWidth();
        intDstRect.right = dstRect->right * context.getWidth();
        intDstRect.top = dstRect->top * context.getHeight();
        intDstRect.bottom = dstRect->bottom * context.getHeight();
        pIntDstRect = &intDstRect;
    }
    
    int result = SDL_RenderCopy(context.getRenderer(), texture, pIntSrcRect, pIntDstRect);
    
    return result;
}


int SDL_RenderCopy(SDL_Renderer* const renderer, SDL_Texture* const texture, const avl::AABB2<int>* const srcAABB, const avl::AABB2<int>* const dstAABB) {
    SDL_Rect* srcRectPtr = nullptr;
    SDL_Rect* dstRectPtr = nullptr;
    SDL_Rect srcRect;
    SDL_Rect dstRect;
    if(srcAABB != nullptr) {
        srcRect = toSDL_Rect(*srcAABB);
        srcRectPtr = &srcRect;
    }
    if(dstAABB != nullptr) {
        dstRect = toSDL_Rect(*dstAABB);
        dstRectPtr = &dstRect;
    }
    
    return SDL_RenderCopy(renderer, texture, srcRectPtr, dstRectPtr);
}

void playSound(Mix_Chunk* const sound) {
    if(Mix_PlayChannel(-1, sound, 0) != 0) {
        throw SDLException(__FILE__, __LINE__, "Mix_PlayChannel()", Mix_GetError());
    }   
}

void playMusic(Mix_Music* const music) {
    if(Mix_PlayMusic(music, -1) != 0) {
        throw SDLException(__FILE__, __LINE__, "Mix_PlayMusic()", Mix_GetError());
    }
}

void pauseMusic() {
    Mix_PauseMusic();
}

void resumeMusic() {
    Mix_ResumeMusic();
}

void stopMusic() {
    Mix_HaltMusic();
}


bool RenderRequest::operator<(const RenderRequest& rhs) const {
    // Sort from most deep to least deep so that deeper ones are rendered first (and then shallow ones on top of them)
    return (depth < rhs.depth);
}


SDLContext::SDLContext(const string& windowTitle, const unsigned int screenWidth, const unsigned int screenHeight, const bool fullscreen)
    : title(windowTitle),
    width(screenWidth),
    height(screenHeight),
    fullscreen(fullscreen)
{
    initSDL();
    
    window = createWindow();
    renderer = createRenderer();
    
    textureManager.reset(new SDLTextureManager(move(unique_ptr<SDLTextureLoader>(new SDLTextureLoader(renderer)))));
    soundManager.reset(new SDLSoundManager(move(unique_ptr<SDLSoundLoader>(new SDLSoundLoader()))));
    musicManager.reset(new SDLMusicManager(move(unique_ptr<SDLMusicLoader>(new SDLMusicLoader()))));
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

SDLTextureManager& SDLContext::getTextureManager() {
    return *textureManager;
}

SDLSoundManager& SDLContext::getSoundManager() {
    return *soundManager;
}

SDLMusicManager& SDLContext::getMusicManager() {
    return *musicManager;
}

void SDLContext::renderTexture(SDL_Texture* const texture, const avl::AABB2<double>* const srcRect, const avl::AABB2<double>* const dstRect, const double depth) {
    RenderRequest request;
    request.texture = texture;
    request.depth = depth;
    
    if(srcRect != nullptr) {
        request.srcRect = *srcRect;
    } else {
        request.srcRect = {0, 0, 1, 1};
    }
    
    if(dstRect != nullptr) {
        request.dstRect = *dstRect;
    } else {
        request.dstRect = {0, 0, 1, 1};
    }
    
    renderQueue.push(request);
}


void SDLContext::present() {
    while(renderQueue.empty() == false) {
        RenderRequest request = renderQueue.top();
        RenderCopy(*this, request.texture, &request.srcRect, &request.dstRect);
        renderQueue.pop();
    }
    
    SDL_RenderPresent(renderer);
}

void SDLContext::initSDL() {
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        cleanup();
        throw SDLException(__FILE__, __LINE__, "SDL_Init()", SDL_GetError());
    }
    
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_JPG;
    if(IMG_Init(imgFlags) != imgFlags) {
        cleanup();
        throw SDLException(__FILE__, __LINE__, "IMG_Init()", IMG_GetError());
    }
    
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        cleanup();
        throw SDLException(__FILE__, __LINE__, "Mix_OpenAudio()", Mix_GetError());
    }
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
    
    Mix_CloseAudio();
    
    Mix_Quit();
    IMG_Quit();
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
    Exception::message = "An error occurred within SDL from function " + function + ": " + errorDesc + ".";
}

const std::string& SDLException::getFunction() const {
    return function;
}

const std::string& SDLException::getError() const {
    return error;
}



SDLTextureLoader::SDLTextureLoader(SDL_Renderer* const renderer)
: renderer(renderer)
{
}

SDL_Texture* SDLTextureLoader::load(const std::string& filePath) {
    return loadTexture(renderer, filePath);
}

void SDLTextureLoader::unload(SDL_Texture* const texture) {
    unloadTexture(texture);
}



Mix_Chunk* SDLSoundLoader::load(const string& filePath) {
    return loadSound(filePath);
}

void SDLSoundLoader::unload(Mix_Chunk* const sound) {
    unloadSound(sound);
}



Mix_Music* SDLMusicLoader::load(const string& filePath) {
    return loadMusic(filePath);
}

void SDLMusicLoader::unload(Mix_Music* const music) {
    unloadMusic(music);
}