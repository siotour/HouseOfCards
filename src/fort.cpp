#include"../include/fort.hpp"
#include"../include/base.hpp"
#include"../include/object.hpp"
#include"../include/room.hpp"
#include<avl/include/utility.hpp>
#include<SDL2/SDL.h>
#include<map>
#include<utility>


using namespace std;
using namespace avl;


const Vec2<double> FortTopLeft = {0.09766, 0.13021};
const Vec2<double> RoomSize = {0.083, 0.110677};


const LocationID FirstLocationID = static_cast<LocationID>(0);
const LocationID LastLocationID = static_cast<LocationID>(FORT_WIDTH * FORT_HEIGHT - 1);
const LocationID InvalidLocationID = static_cast<LocationID>(-1);



bool isValid(const LocationID id) {
    const size_t rawID = static_cast<const size_t>(id);
    return (rawID >= FirstLocationID &&
            rawID <= LastLocationID);
}

size_t getRoomColumn(const LocationID location) {
    //avlVerify(isValid(location));
    return location % FORT_WIDTH;
}

size_t getRoomRow(const LocationID location) {
    //avlVerify(isValid(location));
    return location / FORT_WIDTH;
}

const Location getLocation(const LocationID id) {
    const size_t row = getRoomRow(id);
    const size_t col = getRoomColumn(id);
    
    Location location;
    location.top = FortTopLeft.y + row * RoomSize.y;
    location.bottom = location.top + RoomSize.y;
    location.left = FortTopLeft.x + col * RoomSize.x;
    location.right = location.left + RoomSize.x;
    
    return location;
}

const LocationID getLeft(const LocationID id) {
    LocationID leftID = InvalidLocationID;
    if(id % FORT_WIDTH > 0) {
        leftID = id - 1;
    }
    return leftID;
}

const LocationID getRight(const LocationID id) {
    LocationID rightID = InvalidLocationID;
    if(id % FORT_WIDTH < FORT_WIDTH - 1) {
        rightID = id + 1;
    }
    return rightID;
}

const LocationID getUp(const LocationID id) {
    LocationID upID = InvalidLocationID;
    if(id >= FORT_WIDTH) {
        upID = id - FORT_WIDTH;
    }
    return upID;
}

const LocationID getDown(const LocationID id) {
    LocationID downID = InvalidLocationID;
    if(id <= LastLocationID - FORT_WIDTH) {
        downID = id + FORT_WIDTH;
    }
    return downID;
}

const LocationMap findCompatibleLocations(const LocationID locationID, const ExitType existingExits, const ExitType newExits) {
    LocationMap compatibleLocations;
    
    // Check for compatibility to the left
    if((existingExits & ET_Left) == ET_Left &&
       (newExits & ET_Right) == ET_Right) {
        const LocationID leftID = getLeft(locationID);
        if(isValid(leftID)) {
            const Location& location = getLocation(leftID);
            compatibleLocations.insert(make_pair(leftID, location));
        }
    }
    
    // Check for compatibility to the right
    if((existingExits & ET_Right) == ET_Right &&
       (newExits & ET_Left) == ET_Left) {
        const LocationID rightID = getRight(locationID);
        if(isValid(rightID)) {
            const Location& location = getLocation(rightID);
            compatibleLocations.insert(make_pair(rightID, location));
        }
    }
    
    // Check for compatibility upward
    if((existingExits & ET_Up) == ET_Up &&
       (newExits & ET_Down) == ET_Down) {
        const LocationID upID = getUp(locationID);
        if(isValid(upID)) {
            const Location& location = getLocation(upID);
            compatibleLocations.insert(make_pair(upID, location));
        }
    }
    
    // Check for compatibility downward
    if((existingExits & ET_Down) == ET_Down &&
       (newExits & ET_Up) == ET_Up) {
        const LocationID downID = getDown(locationID);
        if(isValid(downID)) {
            const Location& location = getLocation(downID);
            compatibleLocations.insert(make_pair(downID, location));
        }
    }
    
    return compatibleLocations;
}




Fort::Fort(SDL_Texture* const highlightTexture)
:   showHighlights(false),
    showPreview(false),
    previewTexture(nullptr),
    highlightTexture(highlightTexture)
{
    
}

Fort::~Fort() {
    
}

void Fort::update(const double deltaTime) {
    for(LocationID location = FirstLocationID; isValid(location) == true; ++location) {
        if(roomMatrix[location].get() != nullptr) {
            roomMatrix[location]->update(deltaTime);
            if(roomMatrix[location]->isDead() == true) {
                roomMatrix[location].reset(nullptr);
            }
        }
    }
}

void Fort::render(SDLContext& context) {
    // Render existing rooms
    for(LocationID location = FirstLocationID; isValid(location) == true; ++location) {
        if(roomMatrix[location].get() != nullptr) {
            roomMatrix[location]->render(context);
        }
    }
    // Render highlighted locations if they're being displayed
    if(showHighlights == true) {
        for(auto currentLocation: highlightedLocations) {
            AABB2<double> rect = currentLocation.second;
            context.renderTexture(highlightTexture, nullptr, &rect, 0.9);
        }
    }
    // Render room preview if there is a preview enabled
    if(showPreview == true) {
        Location location = getLocation(previewLocation);
        context.renderTexture(previewTexture, NULL, &location, 0.8);
    }
}

bool Fort::handleEvent(const Event& event) {
    bool eventHandled = false;
    
    for(LocationID location = FirstLocationID; isValid(location) == true; ++location) {
        if(roomMatrix[location].get() != nullptr) {
            eventHandled = roomMatrix[location]->handleEvent(event);
            if(eventHandled == true) {
                break;
            }
        }
    }
    
    return eventHandled;
}

LocationMap Fort::showRoomLocations(const Room& room) {
    showHighlights = true;
    highlightedLocations.clear();
    
    if(roomMatrixEmpty() == true) {
        // Fortress is empty
        highlightedLocations.insert(make_pair(12, getLocation(12)));
        highlightedLocations.insert(make_pair(13, getLocation(13)));
        highlightedLocations.insert(make_pair(14, getLocation(14)));
        highlightedLocations.insert(make_pair(15, getLocation(15)));
    } else {
        // Fortress isn't empty
        for(LocationID location = FirstLocationID; isValid(location); ++location) {
            if(roomMatrix[location].get() != nullptr) {
                const Room& currentRoom = *roomMatrix[location];
                LocationMap currentLocations = findCompatibleLocations(location, currentRoom.getExits(), room.getExits());
                highlightedLocations.insert(currentLocations.begin(), currentLocations.end());
                cullHighlights(highlightedLocations);
            }
        }
    }
    return highlightedLocations;
}

void Fort::hideRoomLocations() {
    showHighlights = false;
    highlightedLocations.clear();
}

void Fort::showRoomPreview(const Room& room, LocationID location) {
    //avlAssert(roomMatrix[col][row].get() == nullptr);
    showPreview = true;
    previewTexture = room.getTexture();
    previewLocation = location;
}

void Fort::hideRoomPreview() {
    showPreview = false;
    previewTexture = nullptr;
}

void Fort::buildRoom(const Room& room, LocationID location) {
    //avlAssert(roomMatrix[col][row].get() == nullptr);
    
    Location roomLocation = getLocation(location);
    Room* const newRoom = new Room(room);
    newRoom->setLocation(roomLocation);
    
    roomMatrix[location].reset(newRoom);
}

bool Fort::roomMatrixEmpty() const {
    bool empty = true;
    for(LocationID location = FirstLocationID; isValid(location); ++location) {
        if(roomMatrix[location].get() != nullptr) {
            empty = false;
            break;
        }
    }
    return empty;
}

void Fort::cullHighlights(LocationMap& highlightedLocations) {
    for(LocationID location = FirstLocationID; isValid(location); ++location) {
        if(roomMatrix[location].get() != nullptr) {
            highlightedLocations.erase(location);
        }
    }
}