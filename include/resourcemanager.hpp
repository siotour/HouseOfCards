/* 
 * File:   resourcemanager.hpp
 * Author: sbachstein
 *
 * Created on April 19, 2014, 9:39 PM
 */

#ifndef RESOURCEMANAGER_HPP
#define	RESOURCEMANAGER_HPP

#include<avl/include/exceptions.hpp>
#include<string>
#include<map>
#include<algorithm>
#include<utility>


template<class Resource>
class ResourceLoader {
public:
    ResourceLoader() = default;
    ~ResourceLoader() = default;
    
    virtual Resource* load(const std::string& filePath) = 0;
    virtual void unload(Resource* const resource) = 0;
};


template<class Resource>
class ResourceManager {
public:
    typedef unsigned int ID;
    
    ResourceManager(ResourceLoader<Resource>& loader);
    ~ResourceManager();
    
    ID load(const std::string& filePath);
    void unload(ID id);
    
    Resource* getByID(ID id);
    
private:
    ID nextID;
    
    ResourceLoader<Resource>& loader;
    typedef std::map<ID, Resource*> ResourceMap;
    ResourceMap resources;
    
};


template<class Resource>
ResourceManager<Resource>::ResourceManager(ResourceLoader<Resource>& loader)
: nextID(1), loader(loader)
{
}

template<class Resource>
ResourceManager<Resource>::~ResourceManager() {
    std::for_each(resources.begin(), resources.end(), [this](typename ResourceMap::value_type& value){
        loader.unload(value.second);
    });
    resources.clear();
}

template<class Resource>
typename ResourceManager<Resource>::ID ResourceManager<Resource>::load(const std::string& filePath) {
    Resource* newResource = loader.load(filePath);
    auto iter = resources.insert(std::make_pair(nextID, newResource));
    ++nextID;
    return iter.first->first;
}

template<class Resource>
void ResourceManager<Resource>::unload(ResourceManager<Resource>::ID id) {
    auto iter = resources.find(id);
    if(iter != resources.end()) {
        loader.unload(iter->second);
        resources.erase(iter);
    }
}

template<class Resource>
Resource* ResourceManager<Resource>::getByID(ResourceManager<Resource>::ID id) {
    auto iter = resources.find(id);
    if(iter == resources.end()) {
        throw avl::InvalidArgumentException(__FILE__, __LINE__, "id", "Must be associated with a valid resource object.");
    }
    return iter->second;
}



#endif	/* RESOURCEMANAGER_HPP */

