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
#include<avl/include/utility.hpp>
#include<SDL2/SDL.h>
#include<map>
#include<memory>

#define FORT_WIDTH 4
#define FORT_HEIGHT 4

class Fort: public Object {
public:
    Fort();
    ~Fort();
    
    void update(const double deltaTime);

    void render(SDLContext& context);
    bool handleEvent(const SDL_Event& event);
    
    LocationMap showRoomLocations(CardID id);
    void hideRoomLocations();
    void showRoomPreview(CardID id, LocationID location);
    void hideRoomPreview();
    void buildRoom(CardID id, LocationID location);
    
private:
    
    bool showHighlights;
    bool showPreview;
    
    LocationMap highlightedLocations;
    LocationID previewLocation;
    SDL_Texture* previewTexture;
    
    std::unique_ptr<Room> roomMatrix[FORT_HEIGHT][FORT_WIDTH];
    
    
};


#endif	/* CASTLE_HPP */

