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
    
    ObjectManager() = default;
    ~ObjectManager() = default;
    
    void add(const ID id, Type* const object);
    void remove(const ID id);
    Type& getByID(const ID id);
    
private:
    std::map<ID, std::unique_ptr<Type>> objects;
};


template<class Type>
void ObjectManager<Type>::add(const ID id, Type* const object) {
    auto result = objects.emplace(id, object);
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
Type& ObjectManager<Type>::getByID(const ID id) {
    auto iter = objects.find(id);
    if(iter == objects.end()) {
        throw avl::InvalidArgumentException(__FILE__, __LINE__, "id", "ID isn't associated with any object.");
    }
    
    return **iter;
}

#endif	/* OBJECTMANAGER_HPP */

