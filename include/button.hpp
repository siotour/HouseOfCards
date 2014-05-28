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
#include<avl/include/utility.hpp>
#include<SDL2/SDL.h>
#include<string>


class Button: public Object {
public:
    Button(const avl::AABB2<double>& position, SDL_Texture* const inactiveTexture, SDL_Texture* const activeTexture);
    ~Button() = default;
    
    void render(SDLContext& context);
    bool handleEvent(const Event& event);
    
private:
    void handleMouseMove(const avl::Vec2<double>& pos);
    void cleanup();
    
    bool isActive;
    avl::AABB2<double> bounds;
    SDL_Texture* inactiveTexture;
    SDL_Texture* activeTexture;
    
    Button() = delete;
    Button& operator=(const Button&) = delete;
};


#endif	/* BUTTONOBJECT_HPP */

