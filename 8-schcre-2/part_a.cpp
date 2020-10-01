/**
 * Project 2a main file.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          David Cree      <cree.d@northeastern.edu>
 * Date:    2020-09-30
 *
 * References
 * ==========
 */

#include <iostream>

#include "deck.h"

int main() {

    Deck deck{};

    std::cout << "Initial deck: " << deck << '\n';

    deck.shuffle();

    std::cout << "Shuffle deck: " << deck << '\n';

    auto top_card = deck.deal();

    if (top_card) {
        std::cout << "Top card " << *top_card << '\n';
        std::cout << "After 1 deal: " << deck << '\n';
    } else {
        std::cout << "The deck was empty\n";
    }



}
