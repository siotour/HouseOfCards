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
    Button(SDL_Texture* inactive, SDL_Texture* active, SDL_Texture* pressed, Mix_Chunk* sound);
    ~Button() = default;
    
    void setPosition(const avl::AABB2<double> position);
    
    void update(const double deltaTime);
    void render(SDLContext& context);
    bool handleEvent(const Event& event);
    
private:
    void handleMouseMove(const avl::Vec2<double>& pos);
    void cleanup();
    
    double pressedDuration;
    
    bool isActive;
    bool isPressed;
 
    avl::AABB2<double> bounds;
    SDL_Texture* inactiveTexture;
    SDL_Texture* activeTexture;
    SDL_Texture* pressedTexture;
    Mix_Chunk* sound;
    
    Button() = delete;
    Button& operator=(const Button&) = delete;
};


#endif	/* BUTTONOBJECT_HPP */