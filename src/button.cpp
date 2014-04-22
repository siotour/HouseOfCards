#include"../include/button.hpp"
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<string>
#include<iostream>


using namespace std;

Button::Button(const SDL_Rect& position, SDL_Texture* const inactiveTexture, SDL_Texture* const activeTexture)
: isActive(false), bounds(position), inactiveTexture(inactiveTexture), activeTexture(activeTexture)
{
}

void Button::render(SDLContext& context) {
    if(isActive == true) {
        SDL_RenderCopy(context.getRenderer(), activeTexture, NULL, &bounds);
    } else {
        SDL_RenderCopy(context.getRenderer(), inactiveTexture, NULL, &bounds);
    }
}

bool Button::handleEvent(const SDL_Event& event) {
    bool isClicked = false;
    
    if(event.type == SDL_MOUSEMOTION) {
        handleMouseMove(event.motion.x, event.motion.y);
    } else if(event.type == SDL_MOUSEBUTTONDOWN) {
        if(event.button.button == SDL_BUTTON_LEFT && isActive == true) {
            isClicked = true;
        }
    }
    
    return isClicked;
}

void Button::handleMouseMove(const short xPos, const short yPos) {
    if(xPos >= bounds.x && xPos <= bounds.x + bounds.w &&
       yPos >= bounds.y && yPos <= bounds.y + bounds.h) {
        isActive = true;
    } else {
        isActive = false;
    }
}
