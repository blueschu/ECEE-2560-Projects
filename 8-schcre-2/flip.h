//
// Created by chand on 10/1/2020.
//

#ifndef ASSIGNMENT_2_FLIP_H
#define ASSIGNMENT_2_FLIP_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include "card.h"
#include "deck.h"

using namespace std;

// Card function declarations

Card::Card(Suit suit, Value value)
{
    this->cardValue = value;
    this->cardSuit = suit;
}

Card::Suit& operator ++(Card::Suit& x)
{
    x = Card::Suit(int(x) + 1);
    return x;
}

Card::Value& operator ++(Card::Value& y)
{
    y = Card::Value(int(y) + 1);
    return y;
}

void Card::setSuit(Suit suit)
{
    this->cardSuit = suit;
}

Card::Suit Card::getSuit()
{
    return cardSuit;
}

Card::Value Card::getValue()
{
    return cardValue;
}

void Card::setValue(Value value){
    this->cardValue = value;
}

// End Card function declarations

// Deck function declarations

Deck::Deck()
{
    maxCards = 52;
    numCards = 0;
    pCards = new vector<Card>(52);
    CreateDeck();
}

Deck::~Deck()
{

}

void Deck::newCard(Card& newCardVar)
{
    pCards->push_back(newCardVar);
}

void Deck::CreateDeck()
{
    for (Card::Value i = Card::Value::Begin; i != Card::Value::End; ++i)
    {
        for (Card::Suit j = Card::Suit::Begin; j != Card::Suit::End; ++j)
        {
            Card card(j, i);
            pCards->push_back(card);

        }
    }
}

// End Deck function declarations


#endif //ASSIGNMENT_2_FLIP_H


