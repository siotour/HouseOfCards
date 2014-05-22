/* 
 * File:   objectmanager.hpp
 * Author: sbachstein
 *
 * Created on May 15, 2014, 12:04 PM
 */

#ifndef OBJECTMANAGER_HPP
#define	OBJECTMANAGER_HPP

#include<map>
#include<memory>
#include<avl/include/exceptions.hpp>

template<class Type>
class ObjectManager {
public:
    typedef unsigned int ID;
    typedef std::map<ID, std::shared_ptr<Type>> MapType;
    
    ObjectManager() = default;
    ~ObjectManager() = default;
    
    void add(const ID id, Type* const object);
    void remove(const ID id);
    std::weak_ptr<Type> getByID(const ID id) const;
    
    const MapType& getMap() const;
    
private:
     MapType objects;
};


template<class Type>
void ObjectManager<Type>::add(const ID id, Type* const object) {
    auto result = objects.emplace(id, std::shared_ptr<Type>(object));
    if(result.second == false) {
        // An object already exists with this ID
        throw avl::InvalidArgumentException(__FILE__, __LINE__, "id", "This ID is already in use.");
    }
}

template<class Type>
void ObjectManager<Type>::remove(const ID id) {
    objects.erase(id);
}

template<class Type>
std::weak_ptr<Type> ObjectManager<Type>::getByID(const ID id) const {
    auto iter = objects.find(id);
    if(iter == objects.end()) {
        throw avl::InvalidArgumentException(__FILE__, __LINE__, "id", "ID isn't associated with any object.");
    }
    
    return std::weak_ptr<Type>(iter->second);
}

template<class Type>
const typename ObjectManager<Type>::MapType& ObjectManager<Type>::getMap() const {
    return objects;
}

#endif	/* OBJECTMANAGER_HPP */