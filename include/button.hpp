/* 
 * File:   buttonobject.hpp
 * Author: sbachstein
 *
 * Created on March 12, 2014, 12:57 PM
 */

#ifndef BUTTONOBJECT_HPP
#define	BUTTONOBJECT_HPP

#include"object.hpp"
#include"sdlutility.hpp"
#include<SDL2/SDL.h>
#include<string>


class Button: public Object {
public:
    Button(const SDL_Rect& position, SDLContext& context, const std::string& inactiveImage, const std::string& activeImage);
    ~Button();
    
    void render(SDLContext& context);
    bool handleEvent(const SDL_Event& event);
    
private:
    void handleMouseMove(const short xPos, const short yPos);
    void cleanup();
    
    bool isActive;
    SDL_Rect bounds;
    SDL_Texture* inactiveTexture;
    SDL_Texture* activeTexture;
    
    Button() = delete;
    Button& operator=(const Button&) = delete;
};


#endif	/* BUTTONOBJECT_HPP */

