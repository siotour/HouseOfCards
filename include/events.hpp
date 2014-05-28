/* 
 * File:   events.hpp
 * Author: sbachstein
 *
 * Created on March 12, 2014, 11:48 AM
 */

#ifndef EVENTS_HPP
#define	EVENTS_HPP

#include"sdlutility.hpp"
#include<avl/include/utility.hpp>
#include<SDL2/SDL.h>
#include<cstdint>

// Forward declaration
union Event;

bool makeEvent(const SDLContext& context, const SDL_Event sdlEvent, Event& event);

enum EventType: uint8_t {
    ET_Invalid = 0,
    ET_Application,
    ET_MouseMove,
    ET_MouseClick,
    ET_MouseScroll,
    ET_KeyPress,
    ET_Quit
};

enum MouseButton: uint8_t {
    MB_Unknown = 0,
    MB_Left,
    MB_Middle,
    MB_Right
};

struct ApplicationEvent {
    EventType type;
    bool active;
};

struct MouseMoveEvent {
    EventType type;
    avl::Vec2<double> relPos;
    avl::Vec2<double> relDelta;
    avl::Vec2<short> absPos;
    avl::Vec2<short> absDelta;
};

struct MouseClickEvent {
    EventType type;
    bool pressed;
    MouseButton button;
    avl::Vec2<double> relPos;
    avl::Vec2<short> absPos;
    
};

struct MouseScrollEvent {
    EventType type;
    short x;
    short y;
};

struct KeyPressEvent {
    EventType type;
    bool pressed;
    SDL_Keysym key;
};

struct QuitEvent {
    EventType type;
};

union Event {
    EventType type;
    MouseMoveEvent mouseMove;
    MouseClickEvent mouseClick;
    MouseScrollEvent mouseScroll;
    KeyPressEvent keyPress;
    ApplicationEvent application;
    QuitEvent quit;
};



#endif	/* EVENTS_HPP */

