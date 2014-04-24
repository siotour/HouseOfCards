/* 
 * File:   cardfactory.hpp
 * Author: sbachstein
 *
 * Created on April 22, 2014, 5:21 PM
 */

#ifndef CARDFACTORY_HPP
#define	CARDFACTORY_HPP


#include"card.hpp"
#include<map>
#include<memory>


class CardFactory {
public:
    CardFactory();
    ~CardFactory() = default;
    
    void addCard(Card* const card);
    Card* makeCard(const CardID id);
    
private:
    std::map<CardID, std::unique_ptr<Card>> cards;
};



#endif	/* CARDFACTORY_HPP */

