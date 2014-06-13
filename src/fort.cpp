#include"../include/fort.hpp"
#include"../include/base.hpp"
#include"../include/object.hpp"
#include"../include/room.hpp"
#include"../include/minion.hpp"
#include"../include/pathfinding.hpp"
#include<avl/include/utility.hpp>
#include<SDL2/SDL.h>
#include<map>
#include<utility>
#include<random>
#include<cassert>


using namespace std;
using namespace avl;


const Vec2<double> FortTopLeft = {0.09766, 0.13021};
const Vec2<double> RoomSize = {0.083, 0.110677};


const RoomCoord FirstLocationID{0, 0};
const RoomCoord LastLocationID{FORT_WIDTH - 1, FORT_HEIGHT - 1};

const double MinionPauseTime = 1.5;

default_random_engine rng;


bool isValid(const RoomCoord id) {
    return (id.x < FORT_WIDTH &&
            id.y < FORT_HEIGHT);
}

const AABB2<double> getLocation(const RoomCoord id) {
    
    AABB2<double> location;
    location.top = FortTopLeft.y + id.y * RoomSize.y;
    location.bottom = location.top + RoomSize.y;
    location.left = FortTopLeft.x + id.x * RoomSize.x;
    location.right = location.left + RoomSize.x;
    
    return location;
}

const RoomCoord getLeft(RoomCoord id) {
    --id.x;
    return id;
}

const RoomCoord getRight(RoomCoord id) {
    ++id.x;
    return id;
}

const RoomCoord getUp(RoomCoord id) {
    --id.y;
    return id;
}

const RoomCoord getDown(RoomCoord id) {
    ++id.y;
    return id;
}

const LocationMap findCompatibleLocations(const RoomCoord room, const ExitType existingExits, const ExitType newExits) {
    LocationMap compatibleLocations;
    
    // Check for compatibility to the left
    if((existingExits & ET_Left) == ET_Left &&
       (newExits & ET_Right) == ET_Right) {
        const RoomCoord leftID = getLeft(room);
        if(isValid(leftID)) {
            const AABB2<double>& location = getLocation(leftID);
            compatibleLocations.insert(make_pair(leftID, location));
        }
    }
    
    // Check for compatibility to the right
    if((existingExits & ET_Right) == ET_Right &&
       (newExits & ET_Left) == ET_Left) {
        const RoomCoord rightID = getRight(room);
        if(isValid(rightID)) {
            const AABB2<double>& location = getLocation(rightID);
            compatibleLocations.insert(make_pair(rightID, location));
        }
    }
    
    // Check for compatibility upward
    if((existingExits & ET_Up) == ET_Up &&
       (newExits & ET_Down) == ET_Down) {
        const RoomCoord upID = getUp(room);
        if(isValid(upID)) {
            const AABB2<double>& location = getLocation(upID);
            compatibleLocations.insert(make_pair(upID, location));
        }
    }
    
    // Check for compatibility downward
    if((existingExits & ET_Down) == ET_Down &&
       (newExits & ET_Up) == ET_Up) {
        const RoomCoord downID = getDown(room);
        if(isValid(downID)) {
            const AABB2<double>& location = getLocation(downID);
            compatibleLocations.insert(make_pair(downID, location));
        }
    }
    
    return compatibleLocations;
}




Fort::Fort(SDL_Texture* const highlightTexture, const Minion* const minionPrototype)
:   showHighlights(false),
    showPreview(false),
    previewTexture(nullptr),
    highlightTexture(highlightTexture),
    minionPrototype(minionPrototype),
        roomsAreDirty(false)
{
}

Fort::~Fort() {
    for(MinionInfo info: minions) {
        delete info.minion;
        info.minion = nullptr;
    }
}

void Fort::update(const double deltaTime) {
    for(auto& row: roomMatrix) {
        for(auto& room: row) {
            if(room.get() != nullptr) {
                room->update(deltaTime);
                if(room->isDead() == true) {
                    room.reset(nullptr);
                    roomsAreDirty = true;
                }
            }
        }
    }
    
    updateMinions(deltaTime);
}

void Fort::render(SDLContext& context) {
    // Render existing rooms
    for(auto& row: roomMatrix) {
        for(auto& room: row) {
            if(room.get() != nullptr) {
                room->render(context);
            }
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
        AABB2<double> location = getLocation(previewLocation);
        context.renderTexture(previewTexture, NULL, &location, 0.6);
    }
    
    for(auto info: minions) {
        info.minion->render(context);
    }
}

bool Fort::handleEvent(const Event& event) {    
    for(auto& row: roomMatrix) {
        for(auto& room: row) {
            if(room.get() != nullptr) {
                if(room->handleEvent(event) == true) {
                    return true;
                }
            }
        }
    }
    
    return false;
}

LocationMap Fort::showRoomLocations(const Room& room) {
    showHighlights = true;
    highlightedLocations.clear();
    
    if(roomMatrixEmpty() == true) {
        // Fortress is empty
        highlightedLocations.insert(make_pair(RoomCoord({0, 3}), getLocation({0, 3})));
        highlightedLocations.insert(make_pair(RoomCoord({1, 3}), getLocation({1, 3})));
        highlightedLocations.insert(make_pair(RoomCoord({2, 3}), getLocation({2, 3})));
        highlightedLocations.insert(make_pair(RoomCoord({3, 3}), getLocation({3, 3})));
    } else {
        // Fortress isn't empty
        for(unsigned int x = 0; x < FORT_WIDTH; ++x) {
            for(unsigned int y = 0; y < FORT_HEIGHT; ++y) {
                if(roomMatrix[x][y].get() != nullptr) {
                    const Room& currentRoom = *roomMatrix[x][y];
                    LocationMap currentLocations = findCompatibleLocations({x, y}, currentRoom.getExits(), room.getExits());
                    highlightedLocations.insert(currentLocations.begin(), currentLocations.end());
                }
            }
        }
        cullHighlights(highlightedLocations);
    }
    return highlightedLocations;
}

void Fort::hideRoomLocations() {
    showHighlights = false;
    highlightedLocations.clear();
}

void Fort::showRoomPreview(const Room& room, RoomCoord location) {
    //avlAssert(roomMatrix[col][row].get() == nullptr);
    showPreview = true;
    previewTexture = room.getTexture();
    previewLocation = location;
}

void Fort::hideRoomPreview() {
    showPreview = false;
    previewTexture = nullptr;
}

void Fort::buildRoom(const Room& room, RoomCoord location) {
    //avlAssert(roomMatrix[col][row].get() == nullptr);
    
    const AABB2<double> roomLocation = getLocation(location);
    Room* const newRoom = new Room(room);
    newRoom->setLocation(roomLocation);
    
    roomMatrix[location.x][location.y].reset(newRoom);
    
    roomsAreDirty = true;
}

RoomCoord Fort::getRandomLocation() const {
    unsigned int numRooms = 0;
    for(auto& row: roomMatrix) {
        for(auto& room: row) {
            if(room.get() != nullptr) {
                ++numRooms;
            }
        }
    }
    
    uniform_int_distribution<unsigned int> dist(0, numRooms - 1);
    
    RoomCoord location{0, 0};
    unsigned int roomNum = dist(rng);
    unsigned int currentRoom = 0;
    
    for(unsigned int x = 0; x < FORT_WIDTH; ++x) {
        for(unsigned int y = 0; y < FORT_HEIGHT; ++y) {
            if(roomMatrix[x][y].get() != nullptr) {
                if(currentRoom == roomNum) {
                    location = {x, y};
                    // Break out of these loops
                    x = FORT_WIDTH;
                    y = FORT_HEIGHT;
                }
                ++currentRoom;
            }
        }
    }
    
    return location;
}

Network<FORT_WIDTH, FORT_HEIGHT> Fort::makeNetwork() const {
    Network<FORT_WIDTH, FORT_HEIGHT> network;
    for(size_t x = 0; x < FORT_WIDTH; ++x) {
        for(size_t y = 0; y < FORT_HEIGHT; ++y) {
            network.nodes[x][y] = (roomMatrix[x][y].get() != nullptr) ? true : false;
        }
    }
    return network;
}

bool Fort::roomMatrixEmpty() const {
    for(auto& row: roomMatrix) {
        for(auto& room: row) {
            if(room.get() != nullptr) {
                return false;
            }
        }
    }
    return true;
}

void Fort::cullHighlights(LocationMap& highlightedLocations) {
    auto i = highlightedLocations.begin();
    while(i != highlightedLocations.end()) {
        if(roomMatrix[i->first.x][i->first.y] != nullptr) {
            i = highlightedLocations.erase(i);
        } else {
            ++i;
        }
    }
}

void Fort::updateMinions(const double& deltaTime) {
    if(roomsAreDirty == true) {
        cullMinions();
        recalculatePaths();
        roomsAreDirty = false;
    }
    
    for(MinionInfo& info: minions) {
        assert(roomMatrix[info.currentRoom.x][info.currentRoom.y].get() != nullptr);
        assert(roomMatrix[info.currentRoom.x][info.currentRoom.y]->getLocation().contains(info.minion->getPosition()) == true);
    
        info.minion->update(deltaTime);
        
        if(info.isPaused == true) {
            info.pauseTime += deltaTime;
            if(info.pauseTime >= MinionPauseTime) {
                info.isPaused = false;
                info.minion->moveTo(roomMatrix[info.nextWaypoint.x][info.nextWaypoint.y]->getWaypoint());
            }
            assert(roomMatrix[info.currentRoom.x][info.currentRoom.y]->getLocation().contains(info.minion->getPosition()) == true);
        } else {
            if(roomMatrix[info.currentRoom.x][info.currentRoom.y]->getLocation().contains(info.minion->getPosition()) == false) {
                assert(roomMatrix[info.nextWaypoint.x][info.nextWaypoint.y]->getLocation().contains(info.minion->getPosition()) == true);
                // Minion has entered the next room
                info.currentRoom = info.nextWaypoint;
            }
            assert(roomMatrix[info.currentRoom.x][info.currentRoom.y]->getLocation().contains(info.minion->getPosition()) == true);
            
            if(info.minion->getPosition() == roomMatrix[info.nextWaypoint.x][info.nextWaypoint.y]->getWaypoint()) {
                // Minion has reached the next waypoint
                if(info.path.empty() == true) {
                    // Minion has made it to the final waypoint -- pause
                    assert(info.currentRoom == info.goalRoom);
                    assert(info.currentRoom == info.nextWaypoint);
                    info.isPaused = true;
                    info.pauseTime = 0;
                    // Choose a new location
                    while(info.path.empty() == true) {
                        info.goalRoom = getRandomLocation();
                        info.path = breadthFirstSearch(info.currentRoom, info.goalRoom, makeNetwork());
                    }
                    assert(info.path.front() == info.currentRoom);
                    if(info.path.size() > 1) {
                        info.path.pop_front();
                    }
                    info.nextWaypoint = info.path.front();
                    info.path.pop_front();
                    // 'moveTo' the current room until we're done being paused
                    info.minion->moveTo(roomMatrix[info.currentRoom.x][info.currentRoom.y]->getWaypoint());
                    assert(roomMatrix[info.currentRoom.x][info.currentRoom.y]->getLocation().contains(info.minion->getPosition()) == true);
                } else {
                    // Move to the next waypoint
                    info.nextWaypoint = info.path.front();
                    info.path.pop_front();
                    //avlAssert(roomMatrix[waypoint].get() != nullptr);
                    info.minion->moveTo(roomMatrix[info.nextWaypoint.x][info.nextWaypoint.y]->getWaypoint());
                    assert(roomMatrix[info.currentRoom.x][info.currentRoom.y]->getLocation().contains(info.minion->getPosition()) == true);
                }   
            }
        }
        assert(roomMatrix[info.currentRoom.x][info.currentRoom.y].get() != nullptr);
        assert(roomMatrix[info.currentRoom.x][info.currentRoom.y]->getLocation().contains(info.minion->getPosition()) == true);
        assert(roomMatrix[info.nextWaypoint.x][info.nextWaypoint.y].get() != nullptr);
    }
}

void Fort::spawnMinion() {
    if(roomMatrixEmpty() == false) {
        MinionInfo info;
        info.pauseTime = 0;
        info.isPaused = true;
        info.minion = new Minion(*minionPrototype);
        
        info.currentRoom = getRandomLocation();
        info.minion->setPosition(roomMatrix[info.currentRoom.x][info.currentRoom.y]->getWaypoint());
        info.minion->moveTo(roomMatrix[info.currentRoom.x][info.currentRoom.y]->getWaypoint());
        
        while(info.path.empty() == true) {
            info.goalRoom = getRandomLocation();
            info.path = breadthFirstSearch(info.currentRoom, info.goalRoom, makeNetwork());
        }
        
        assert(info.currentRoom == info.path.front());
        if(info.path.size() > 1) {
            info.path.pop_front();
        }
        info.nextWaypoint = info.path.front();
        info.path.pop_front();
        
        assert(roomMatrix[info.nextWaypoint.x][info.nextWaypoint.y].get() != nullptr);
        
        minions.push_back(info);
        
        assert(roomMatrix[info.currentRoom.x][info.currentRoom.y].get() != nullptr);
        assert(roomMatrix[info.currentRoom.x][info.currentRoom.y]->getLocation().contains(info.minion->getPosition()) == true);
    }
}

void Fort::cullMinions() {
    auto info = minions.begin();
    while(info != minions.end()) {
        if(roomMatrix[info->currentRoom.x][info->currentRoom.y].get() == nullptr) {
            // This minion isn't contained in any room... Delete it.
            delete info->minion;
            info->minion = nullptr;
            info = minions.erase(info);
        } else {
            assert(roomMatrix[info->currentRoom.x][info->currentRoom.y].get() != nullptr);
            assert(roomMatrix[info->currentRoom.x][info->currentRoom.y]->getLocation().contains(info->minion->getPosition()) == true);
            ++info;
        }
    }
    
}

void Fort::recalculatePaths() {
    for(MinionInfo& info: minions) {
        assert(roomMatrix[info.currentRoom.x][info.currentRoom.y].get() != nullptr);
        assert(roomMatrix[info.currentRoom.x][info.currentRoom.y]->getLocation().contains(info.minion->getPosition()) == true);
        
        bool pathIsBroken = false;
        if(roomMatrix[info.nextWaypoint.x][info.nextWaypoint.y].get() == nullptr ||
           roomMatrix[info.goalRoom.x][info.goalRoom.y].get() == nullptr) {
            pathIsBroken = true;
        } else {
            for(const RoomCoord& room: info.path) {
                if(roomMatrix[room.x][room.y].get() == nullptr) {
                    pathIsBroken = true;
                    break;
                }
            }
        }
        
        if(pathIsBroken == true) {
            // Try to resume the path from the 'next waypoint' since the minion is already walking that way
            if(roomMatrix[info.nextWaypoint.x][info.nextWaypoint.y].get() != nullptr) {
                info.path = breadthFirstSearch(info.nextWaypoint, info.goalRoom, makeNetwork());
                // Find a new goal if necessary
                while(info.path.empty() == true) {
                    info.goalRoom = getRandomLocation();
                    info.path = breadthFirstSearch(info.nextWaypoint, info.goalRoom, makeNetwork());
                }
                assert(info.path.front() == info.nextWaypoint);
                info.path.pop_front();
            } else {
                // We can't resume from the next waypoint. We'll have to backtrack to the current room and proceed from there
                info.nextWaypoint = info.currentRoom;
                info.path = breadthFirstSearch(info.currentRoom, info.goalRoom, makeNetwork());
                // Find a new goal if necessary
                while(info.path.empty() == true) {
                    info.goalRoom = getRandomLocation();
                    info.path = breadthFirstSearch(info.currentRoom, info.goalRoom, makeNetwork());
                }
                assert(info.path.front() == info.currentRoom);
                info.path.pop_front();
            }
        }
        
        if(info.isPaused == false) {
            info.minion->moveTo(roomMatrix[info.nextWaypoint.x][info.nextWaypoint.y]->getWaypoint());
        }
        
        assert(roomMatrix[info.currentRoom.x][info.currentRoom.y].get() != nullptr);
        assert(roomMatrix[info.currentRoom.x][info.currentRoom.y]->getLocation().contains(info.minion->getPosition()) == true);
        assert(roomMatrix[info.nextWaypoint.x][info.nextWaypoint.y].get() != nullptr);
        assert(roomMatrix[info.goalRoom.x][info.goalRoom.y].get() != nullptr);
    }
}