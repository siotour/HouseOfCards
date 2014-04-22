#ifndef SDLUTILITY_HPP
#define	SDLUTILITY_HPP

#include"resourcemanager.hpp"
#include<avl/include/utility.hpp>
#include<avl/include/exceptions.hpp>
#include<SDL2/SDL.h>
#include<string>


typedef ResourceManager<SDL_Texture> SDLTextureManager;

SDL_Texture* loadTexture(SDL_Renderer* const renderer, const std::string& filePath);
void unloadTexture(SDL_Texture* const texture);

const SDL_Rect toSDL_Rect(const avl::AABB2<int>& original);

int SDL_RenderCopy(SDL_Renderer* renderer, SDL_Texture* texture, const avl::AABB2<int>* srcRect, const avl::AABB2<int>* dstRect);


class SDLContext {
public:
    SDLContext(const std::string& windowTitle, const unsigned int screenWidth, const unsigned int screenHeight, const bool fullscreen);
    ~SDLContext();
    
    unsigned int getWidth() const;
    unsigned int getHeight() const;
    bool isFullscreen() const;
    
    SDL_Window* getWindow();
    SDL_Renderer* getRenderer();
    
private:
    void initSDL();
    void cleanup();
    SDL_Window* createWindow();
    SDL_Renderer* createRenderer();
    
    std::string title;
    unsigned int width;
    unsigned int height;
    bool fullscreen;
    
    SDL_Window* window;
    SDL_Renderer* renderer;
    
    SDLContext() = delete;
    SDLContext& operator=(const SDLContext&) = delete;
};


class SDLException: public avl::Exception {
public:
    SDLException(const std::string& fileName, const unsigned int lineNum, const std::string& funcName, const std::string& errorDesc);
    ~SDLException() = default;
    
    const std::string& getFunction() const;
    const std::string& getError() const;
    
private:
    const std::string function;
    const std::string error;
    
    SDLException() = delete;
    SDLException& operator=(const SDLException&) = delete;
};


class SDLTextureLoader: public ResourceLoader<SDL_Texture> {
public:
    SDLTextureLoader(SDLContext& context);
    ~SDLTextureLoader() = default;
    
    SDL_Texture* load(const std::string& filePath);
    void unload(SDL_Texture* const texture);
    
private:
    SDLContext& context;
};


#endif	/* SDLUTILITY_HPP */

