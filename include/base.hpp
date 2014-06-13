/* 
 * File:   base.hpp
 * Author: sbachstein
 *
 * Created on April 18, 2014, 11:25 AM
 */

#ifndef BASE_HPP
#define	BASE_HPP

#include"scene.hpp"
#include<avl/include/utility.hpp>
#include<SDL2/SDL.h>
#include<map>
#include<functional>

typedef unsigned short CardID;

typedef avl::Vec2<unsigned int> RoomCoord;
typedef std::map<RoomCoord, avl::AABB2<double>> LocationMap;


const SceneType ST_Quit = 0;
const SceneType ST_MainMenu = 1;
const SceneType ST_Game = 2;

namespace std {
template<>
struct less<RoomCoord> {
    bool operator()(const RoomCoord& lhs, const RoomCoord& rhs);
};
}


#endif	/* BASE_HPP */

