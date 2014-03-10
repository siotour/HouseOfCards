#ifndef SDLUTILITY_HPP
#define	SDLUTILITY_HPP

#include<avl/include/exceptions.hpp>
#include<SDL2/SDL.h>
#include<string>


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


#endif	/* SDLUTILITY_HPP */

