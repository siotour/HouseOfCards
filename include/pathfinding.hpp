/* 
 * File:   pathfinding.hpp
 * Author: sbachstein
 *
 * Created on June 1, 2014, 1:55 PM
 */

#ifndef PATHFINDING_HPP
#define	PATHFINDING_HPP

#include<avl/include/utility.hpp>
#include<list>
#include<set>
#include<queue>
#include<stack>
#include<functional>

typedef avl::Vec2<unsigned int> Vertex;
template<unsigned int Width, unsigned int Height>
struct Network{
    bool nodes[Width][Height];
};
typedef std::list<Vertex> Path;

struct VertexLess : public std::binary_function<Vertex, Vertex, bool> {
    bool operator()(const Vertex& lhs, const Vertex& rhs) {
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
};


template<unsigned int Width, unsigned int Height>
Path breadthFirstSearch(const Vertex start, const Vertex end, const Network<Width, Height>& network);

template<unsigned int Width, unsigned int Height>
bool bfs(const Vertex start, const Vertex end, const Network<Width, Height>& network, Path& path, std::set<Vertex, VertexLess>& processedVertices);

template<unsigned int Width, unsigned int Height>
const std::vector<Vertex> neighbors(const Vertex& vertex, const Network<Width, Height>& network);

// path.size() == 0 => no path from start to end
// path.size() == 1 => start == end
// path.size() > 1  => path goes from start to end with start and end included
template<unsigned int Width, unsigned int Height>
Path breadthFirstSearch(const Vertex start, const Vertex end, const Network<Width, Height>& network) {
    Path path;
    std::set<Vertex, VertexLess> processedVertices;
    processedVertices.insert(start);
    
    bfs(start, end, network, path, processedVertices);
    
    return path;
}

template<unsigned int Width, unsigned int Height>
bool bfs(const Vertex start, const Vertex end, const Network<Width, Height>& network, Path& path, std::set<Vertex, VertexLess>& processedVertices) {
    if(start == end) {
        path.push_front(end);
        return true;
    }
    
    Vertex currentVertex = start;
    std::queue<Vertex> unprocessedVertices;
    
    
    do{
        
        for(Vertex neighbor : neighbors<Width, Height>(currentVertex, network)) {
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
const std::vector<Vertex> neighbors(const Vertex& vertex, const Network<Width, Height>& network) {
    std::vector<Vertex> neighbors;
    
    // Left neighbor
    if(vertex.x > 0 && network.nodes[vertex.x - 1][vertex.y] == true) {
        neighbors.push_back(Vertex{vertex.x - 1, vertex.y});
    }
    // Top neighbor
    if(vertex.y > 0 && network.nodes[vertex.x][vertex.y - 1] == true) {
        neighbors.push_back(Vertex{vertex.x, vertex.y - 1});
    }
    // Right neighbor
    if(vertex.x < Width - 1 && network.nodes[vertex.x + 1][vertex.y] == true) {
        neighbors.push_back(Vertex{vertex.x + 1, vertex.y});
    }
    // Bottom neighbor
    if(vertex.y < Height - 1 && network.nodes[vertex.x][vertex.y + 1]== true) {
        neighbors.push_back(Vertex{vertex.x, vertex.y + 1});
    }
    
    return neighbors;
}


#endif	/* PATHFINDING_HPP */

