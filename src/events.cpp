#include"../include/events.hpp"
#include"../include/sdlutility.hpp"
#include<avl/include/utility.hpp>
#include<SDL2/SDL.h>


MouseButton toMouseButton(const uint8_t rawButton) {
    MouseButton button = MB_Unknown;
    switch(rawButton) {
        case SDL_BUTTON_LEFT:
            button = MB_Left;
            break;
        case SDL_BUTTON_MIDDLE:
            button = MB_Middle;
            break;
        case SDL_BUTTON_RIGHT:
            button = MB_Right;
            break;
    }
    
    return button;
}

bool makeEvent(const SDLContext& context, const SDL_Event sdlEvent, Event& event) {
    bool eventModified = true;
    
    switch(sdlEvent.type) {
        case SDL_MOUSEMOTION:
            event.type = ET_MouseMove;
            event.mouseMove.absPos.x = sdlEvent.motion.x;
            event.mouseMove.absPos.y = sdlEvent.motion.y;
            event.mouseMove.relPos.x = static_cast<double>(sdlEvent.motion.x) / static_cast<double>(context.getWidth());
            event.mouseMove.relPos.y = static_cast<double>(sdlEvent.motion.y) / static_cast<double>(context.getHeight());
            event.mouseMove.absDelta.x = sdlEvent.motion.xrel;
            event.mouseMove.absDelta.y = sdlEvent.motion.yrel;
            event.mouseMove.relDelta.x = static_cast<double>(sdlEvent.motion.xrel) / static_cast<double>(context.getWidth());
            event.mouseMove.relDelta.y = static_cast<double>(sdlEvent.motion.yrel) / static_cast<double>(context.getHeight());
            break;
        case SDL_MOUSEBUTTONDOWN:
            // Falls through
        case SDL_MOUSEBUTTONUP:
            if(sdlEvent.button.button == SDL_BUTTON_LEFT ||
               sdlEvent.button.button == SDL_BUTTON_MIDDLE ||
               sdlEvent.button.button == SDL_BUTTON_RIGHT) {
                event.type = ET_MouseClick;
                event.mouseClick.button = toMouseButton(sdlEvent.button.button);
                event.mouseClick.pressed = (sdlEvent.button.state == SDL_PRESSED) ? true : false;
                event.mouseClick.absPos.x = sdlEvent.button.x;
                event.mouseClick.absPos.y = sdlEvent.button.y;
                event.mouseClick.relPos.x = static_cast<double>(sdlEvent.button.x) / static_cast<double>(context.getWidth());
                event.mouseClick.relPos.y = static_cast<double>(sdlEvent.button.y) / static_cast<double>(context.getHeight());
            }
            break;
        case SDL_MOUSEWHEEL:
            event.type = ET_MouseScroll;
            event.mouseScroll.x = sdlEvent.wheel.x;
            event.mouseScroll.y = sdlEvent.wheel.y;
            break;
        case SDL_WINDOWEVENT:
            if(sdlEvent.window.event == SDL_WINDOWEVENT_ENTER ||
               sdlEvent.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
                event.type = ET_Application;
                event.application.active = true;
            } else if(sdlEvent.window.event == SDL_WINDOWEVENT_LEAVE ||
                      sdlEvent.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
                event.type = ET_Application;
                event.application.active = false;
            }
            break;
        case SDL_KEYDOWN:
            // Falls through
        case SDL_KEYUP:
            event.type = ET_KeyPress;
            event.keyPress.key = sdlEvent.key.keysym;
            event.keyPress.pressed = (sdlEvent.key.type == SDL_KEYDOWN) ? true : false;
            break;
        case SDL_QUIT:
            event.type = ET_Quit;
            break;
        default:
            // No event update
            eventModified = false;
    }
    
    return eventModified;
}
