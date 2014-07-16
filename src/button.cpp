#include"../include/button.hpp"
#include"../include/sdlutility.hpp"
#include<avl/include/utility.hpp>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_mixer.h>
#include<string>


using namespace std;
using namespace avl;

const double PressedPeriod = 0.1;

Button::Button(SDL_Texture* inactive, SDL_Texture* active, SDL_Texture* pressed, Mix_Chunk* sound)
: isActive(false), isPressed(false), inactiveTexture(inactive), activeTexture(active), pressedTexture(pressed), sound(sound)
{
    bounds = {0, 0, 0, 0};
}

void Button::setPosition(const avl::AABB2<double> position) {
    bounds = position;
}

void Button::update(const double deltaTime) {
    if(isPressed == true) {
        pressedDuration += deltaTime;
    }
    
    if(pressedDuration >= PressedPeriod) {
        isPressed = false;
    }
}

void Button::render(SDLContext& context) {
    if(isPressed == true) {
        context.renderTexture(pressedTexture, nullptr, &bounds, 0.5);
    }
    else if(isActive == true) {
        context.renderTexture(activeTexture, nullptr, &bounds, 0.5);
    } else {
        context.renderTexture( inactiveTexture, nullptr, &bounds, 0.5);
    }
}

bool Button::handleEvent(const Event& event) {
    bool newlyPressed = false;
    
    if(event.type == ET_MouseMove) {
        handleMouseMove(event.mouseMove.relPos);
    } else if(event.type == ET_MouseClick) {
        if(event.mouseClick.button == MB_Left && event.mouseClick.pressed == true && isActive == true) {
            newlyPressed = true;
            isPressed = true;
            pressedDuration = 0;
            playSound(sound);
        }
    }
    
    return newlyPressed;
}

void Button::handleMouseMove(const Vec2<double>& pos) {
    if(bounds.contains(pos) == true) {
        isActive = true;
    } else {
        isActive = false;
    }
}
