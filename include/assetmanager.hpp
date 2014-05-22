/* 
 * File:   assetmanager.hpp
 * Author: sbachstein
 *
 * Created on April 19, 2014, 9:39 PM
 */

#ifndef ASSETMANAGER_HPP
#define	ASSETMANAGER_HPP

#include<avl/include/exceptions.hpp>
#include<string>
#include<map>
#include<algorithm>
#include<utility>
#include<memory>


template<class Asset>
class AssetLoader {
public:
    AssetLoader() = default;
    ~AssetLoader() = default;
    
    virtual Asset* load(const std::string& filePath) = 0;
    virtual void unload(Asset* const asset) = 0;
};


template<class Asset>
class AssetManager {
public:
    typedef unsigned int ID;
    
    AssetManager(std::unique_ptr<AssetLoader<Asset>> loader);
    ~AssetManager();
    
    ID load(const std::string& filePath);
    void unload(ID id);
    
    Asset* getByID(ID id);
    
private:
    ID nextID;
    
    std::unique_ptr<AssetLoader<Asset>> loader;
    typedef std::map<ID, Asset*> AssetMap;
    AssetMap assets;
    
};


template<class Asset>
AssetManager<Asset>::AssetManager(std::unique_ptr<AssetLoader<Asset>> loader)
: nextID(1), loader(std::move(loader))
{
    //avlAssert(loader != nullptr);
}

template<class Asset>
AssetManager<Asset>::~AssetManager() {
    std::for_each(assets.begin(), assets.end(), [this](typename AssetMap::value_type& value){
        loader->unload(value.second);
    });
    assets.clear();
}

template<class Asset>
typename AssetManager<Asset>::ID AssetManager<Asset>::load(const std::string& filePath) {
    Asset* newAsset = loader->load(filePath);
    auto iter = assets.insert(std::make_pair(nextID, newAsset));
    ++nextID;
    return iter.first->first;
}

template<class Asset>
void AssetManager<Asset>::unload(AssetManager<Asset>::ID id) {
    auto iter = assets.find(id);
    if(iter != assets.end()) {
        loader->unload(iter->second);
        assets.erase(iter);
    }
}

template<class Asset>
Asset* AssetManager<Asset>::getByID(AssetManager<Asset>::ID id) {
    auto iter = assets.find(id);
    if(iter == assets.end()) {
        throw avl::InvalidArgumentException(__FILE__, __LINE__, "id", "Must be associated with a valid asset object.");
    }
    return iter->second;
}



#endif	/* ASSETMANAGER_HPP */

