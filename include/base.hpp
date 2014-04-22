/* 
 * File:   base.hpp
 * Author: sbachstein
 *
 * Created on April 18, 2014, 11:25 AM
 */

#ifndef BASE_HPP
#define	BASE_HPP

#include<avl/include/utility.hpp>
#include<SDL2/SDL.h>
#include<map>

typedef unsigned short CardID;

typedef unsigned short LocationID;
typedef avl::AABB2<int> Location;
typedef std::map<LocationID, Location> LocationMap;


#endif	/* BASE_HPP */

