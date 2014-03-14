/* 
 * File:   events.hpp
 * Author: sbachstein
 *
 * Created on March 12, 2014, 11:48 AM
 */

#ifndef EVENTS_HPP
#define	EVENTS_HPP

#include<cstdint>

/* Alternative event paradigm:
 void Object::handleEvent(uint8_t eventType);
 * 
 * Globals:
 * MousePos;
 * MouseDeltaPos;
 * KeyboardState;
 * ...
 */

/*

enum EventType: uint8_t {
    ET_Invalid = 0,
    ET_MouseMove,
    ET_MouseClick,
    ET_MouseScroll,
    ET_KeyPress,
    ET_WindowEvent
};

struct MouseMoveEvent {
    EventType type;
    Vec2<short> newPos;
    Vec2<short> deltaPos;
};

struct MouseClickEvent {
    EventType type;
    Vec2<short> pos;
    
};

union Event {
    EventType type;
    MouseMoveEvent mouseMove;
    MouseClickEvent mouseClick;
    MouseScrollEvent mouseScroll;
    KeyPressEvent keyPress;
    WindowEvent windowEvent;
};

*/

#endif	/* EVENTS_HPP */

