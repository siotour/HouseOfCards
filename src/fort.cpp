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


const Vec2<int> FortTopLeft = {100, 100};
const int RoomWidth = 85;
const int RoomHeight = 85;


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
    location.top = FortTopLeft.y + row * RoomHeight;
    location.bottom = location.top + RoomHeight;
    location.left = FortTopLeft.x + col * RoomWidth;
    location.right = location.left + RoomWidth;
    
    return location;
}

const LocationID getLeft(const LocationID id) {
    LocationID leftID = InvalidLocationID;
    if(id != 0) {
        leftID = id - 1;
    }
    return leftID;
}

const LocationID getRight(const LocationID id) {
    LocationID rightID = InvalidLocationID;
    if(id != FORT_WIDTH - 1) {
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
    if(id < FORT_WIDTH * (FORT_HEIGHT - 1)) {
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
    
}

void Fort::render(SDLContext& context) {
    // Render existing rooms
    for(size_t row = 0; row < FORT_HEIGHT; ++row) {
        for(size_t col = 0; col < FORT_WIDTH; ++col) {
            if(roomMatrix[row][col].get() != nullptr) {
                roomMatrix[row][col]->render(context);
            }
        }
    }
    // Render highlighted locations if they're being displayed
    if(showHighlights == true) {
        for(auto currentLocation: highlightedLocations) {
            SDL_Rect rect = toSDL_Rect(currentLocation.second);
            SDL_RenderCopy(context.getRenderer(), highlightTexture, nullptr, &rect);
        }
    }
    // Render room preview if there is a preview enabled
    if(showPreview == true) {
        Location location = getLocation(previewLocation);
        SDL_Rect previewRect = toSDL_Rect(location);
        SDL_RenderCopy(context.getRenderer(), previewTexture, NULL, &previewRect);
    }
}

bool Fort::handleEvent(const SDL_Event& event) {
    return false;
}

LocationMap Fort::showRoomLocations(const Room& room) {
    showHighlights = true;
    highlightedLocations.clear();
    
    if(roomMatrixEmpty() == true) {
        // Fortress is empty
        highlightedLocations.insert(make_pair(0, getLocation(0)));
    } else {
        // Fortress isn't empty
        for(LocationID location = FirstLocationID; isValid(location); ++location) {
            size_t row = getRoomRow(location);
            size_t col = getRoomColumn(location);
            if(roomMatrix[row][col].get() != nullptr) {
                const Room& currentRoom = *roomMatrix[row][col];
                LocationMap currentLocations = findCompatibleLocations(location, currentRoom.getExits(), room.getExits());
                highlightedLocations.insert(currentLocations.begin(), currentLocations.end());
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
    const size_t col = getRoomColumn(location);
    const size_t row = getRoomRow(location);
    //avlAssert(roomMatrix[col][row].get() == nullptr);
    
    Location roomLocation = getLocation(location);
    Room* const newRoom = new Room(room);
    newRoom->setLocation(roomLocation);
    
    roomMatrix[row][col].reset(newRoom);
}

bool Fort::roomMatrixEmpty() const {
    bool empty = true;
    for(LocationID location = FirstLocationID; isValid(location); ++location) {
        size_t row = getRoomRow(location);
        size_t col = getRoomColumn(location);
        if(roomMatrix[row][col].get() != nullptr) {
            empty = false;
            break;
        }
    }
    return empty;
}