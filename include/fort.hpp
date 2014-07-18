/* 
 * File:   fort.hpp
 * Author: sbachstein
 *
 * Created on March 14, 2014, 11:55 AM
 */

#ifndef CASTLE_HPP
#define	CASTLE_HPP

#include"object.hpp"
#include"room.hpp"
#include"base.hpp"
#include"events.hpp"
#include"minion.hpp"
#include"pathfinding.hpp"
#include<avl/include/utility.hpp>
#include<SDL2/SDL.h>
#include<SDL2/SDL_mixer.h>
#include<map>
#include<memory>
#include<list>

#define FORT_WIDTH 3
#define FORT_HEIGHT 4


struct MinionInfo {
    Minion* minion;
    RoomCoord currentRoom;
    RoomCoord nextWaypoint;
    RoomCoord goalRoom;
    Path path;
    double pauseTime;
    bool isPaused;
};


class Fort: public Object {
public:
    Fort(SDL_Texture* const highlightTexture, Mix_Chunk* roomBuildSound, Mix_Chunk* roomDestroySound, const Minion* const minionPrototype);
    ~Fort();
    
    void update(const double deltaTime);

    void render(SDLContext& context);
    bool handleEvent(const Event& event);
    
    LocationMap showRoomLocations(const Room& room);
    void hideRoomLocations();
    void showRoomPreview(const Room& room, RoomCoord location);
    void hideRoomPreview();
    void buildRoom(const Room& room, RoomCoord location);
    void spawnMinion();
    
    
private:
    RoomCoord getRandomLocation() const;
    Network<FORT_WIDTH, FORT_HEIGHT> makeNetwork() const;
    bool roomMatrixEmpty() const;
    void cullHighlights(LocationMap& highlightedLocations);
    void updateMinions(const double& deltaTime);
    void cullMinions();
    void recalculatePaths();
    
    
    bool showHighlights;
    bool showPreview;
    
    bool roomsAreDirty;
    std::list<MinionInfo> minions;
    std::unique_ptr<const Minion> minionPrototype;
    
    LocationMap highlightedLocations;
    RoomCoord previewLocation;
    SDL_Texture* previewTexture;
    
    SDL_Texture* highlightTexture;
    
    Mix_Chunk* roomBuildSound;
    Mix_Chunk* roomDestroySound;
    
    std::array<std::array<std::unique_ptr<Room>, FORT_HEIGHT>, FORT_WIDTH> roomMatrix;
    
    
};


#endif	/* CASTLE_HPP */

