#include"../include/base.hpp"
#include<functional>

namespace std {
bool less<RoomCoord>::operator()(const RoomCoord& lhs, const RoomCoord& rhs) {
    if(lhs.x < rhs.x) {
        return true;
    }
    if(lhs.x > rhs.x) {
        return false;
    }
    if(lhs.y < rhs.y) {
        return true;
    }
    if(lhs.y > rhs.y) {
        return false;
    }
    return false;
}
}