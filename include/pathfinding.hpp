/* 
 * File:   pathfinding.hpp
 * Author: sbachstein
 *
 * Created on June 1, 2014, 1:55 PM
 */

#ifndef PATHFINDING_HPP
#define	PATHFINDING_HPP

#include"base.hpp"
#include<avl/include/utility.hpp>
#include<list>
#include<set>
#include<queue>
#include<stack>
#include<functional>

template<unsigned int Width, unsigned int Height>
struct Network{
    bool nodes[Width][Height];
};
typedef std::list<RoomCoord> Path;


template<unsigned int Width, unsigned int Height>
Path breadthFirstSearch(const RoomCoord start, const RoomCoord end, const Network<Width, Height>& network);

template<unsigned int Width, unsigned int Height>
bool bfs(const RoomCoord start, const RoomCoord end, const Network<Width, Height>& network, Path& path, std::set<RoomCoord>& processedVertices);

template<unsigned int Width, unsigned int Height>
const std::vector<RoomCoord> neighbors(const RoomCoord& vertex, const Network<Width, Height>& network);

// path.size() == 0 => no path from start to end
// path.size() == 1 => start == end
// path.size() > 1  => path goes from start to end with start and end included
template<unsigned int Width, unsigned int Height>
Path breadthFirstSearch(const RoomCoord start, const RoomCoord end, const Network<Width, Height>& network) {
    Path path;
    
    if(network.nodes[start.x][start.y] == true &&
       network.nodes[end.x][end.y] == true) {
    
        std::set<RoomCoord> processedVertices;
        processedVertices.insert(start);

        bfs(start, end, network, path, processedVertices);
    }
    
    return path;
}

template<unsigned int Width, unsigned int Height>
bool bfs(const RoomCoord start, const RoomCoord end, const Network<Width, Height>& network, Path& path, std::set<RoomCoord>& processedVertices) {
    if(start == end) {
        path.push_front(end);
        return true;
    }
    
    RoomCoord currentVertex = start;
    std::queue<RoomCoord> unprocessedVertices;
    
    do{
        
        for(RoomCoord neighbor : neighbors<Width, Height>(currentVertex, network)) {
            if(processedVertices.find(neighbor) == processedVertices.end()) {
                processedVertices.size();
                unprocessedVertices.push(neighbor);
                processedVertices.insert(neighbor);
            }
        }
        
        if(unprocessedVertices.empty() == false) {
            currentVertex = unprocessedVertices.front();
            unprocessedVertices.pop();

            if(bfs(currentVertex, end, network, path, processedVertices) == true) {
                path.push_front(start);
                return true;
            }
        }
    } while(unprocessedVertices.empty() == false);
    
    return false;
}


template<unsigned int Width, unsigned int Height>
const std::vector<RoomCoord> neighbors(const RoomCoord& vertex, const Network<Width, Height>& network) {
    std::vector<RoomCoord> neighbors;
    
    // Left neighbor
    if(vertex.x > 0 && network.nodes[vertex.x - 1][vertex.y] == true) {
        neighbors.push_back(RoomCoord{vertex.x - 1, vertex.y});
    }
    // Top neighbor
    if(vertex.y > 0 && network.nodes[vertex.x][vertex.y - 1] == true) {
        neighbors.push_back(RoomCoord{vertex.x, vertex.y - 1});
    }
    // Right neighbor
    if(vertex.x < Width - 1 && network.nodes[vertex.x + 1][vertex.y] == true) {
        neighbors.push_back(RoomCoord{vertex.x + 1, vertex.y});
    }
    // Bottom neighbor
    if(vertex.y < Height - 1 && network.nodes[vertex.x][vertex.y + 1]== true) {
        neighbors.push_back(RoomCoord{vertex.x, vertex.y + 1});
    }
    
    return neighbors;
}


#endif	/* PATHFINDING_HPP */

