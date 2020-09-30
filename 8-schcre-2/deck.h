/**
 * Deck class for Project 2a.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          David Cree      <cree.d@northeastern.edu>
 * Date:    2020-09-30
 *
 * References
 * ==========
 *
 *  [1]: https://en.cppreference.com/w/cpp/algorithm/random_shuffle
 */

#ifndef EECE_2560_PROJECTS_DECK_H
#define EECE_2560_PROJECTS_DECK_H

#include <algorithm>            // for std::random_shuffle
#include <forward_list>         // temporary: for standard list container
#include <ostream>
#include <random>               // for random number generation
#include <vector>

#include "card.h"

template<typename List = std::forward_list<Card>>
class Deck {

    /// The number of playing cards in a full deck.
    constexpr static std::size_t FULL_DECK_SIZE{Card::ALL_RANKS.size() * Card::ALL_SUITS.size()};

    /// This deck's list of cards
    List m_card_list;

  public:
    using iterator = typename List::iterator;
    using const_iterator = typename List::const_iterator;

    /**
     * Creates a Deck will all 52 cards in their sorted order.
     */
    Deck()
    {
        auto it = m_card_list.before_begin();

        for (auto rank : Card::ALL_RANKS) {
            for (auto suit : Card::ALL_SUITS) {
                // Use emplace to construct the card in place rather than copying a temporary.
                it = m_card_list.emplace_after(it, rank, suit);
            }
        }
    }

    template<typename R = std::default_random_engine>
    void shuffle(R entropy_source = R(default_random_seed()))
    {
        // We cannot use a fixed size array since we do not provide
        // a default constructor for Card
        std::vector<Card> shuffle_buff(std::begin(m_card_list), std::end(m_card_list));

        // Empty the card list.
        m_card_list.clear();

        std::shuffle(std::begin(shuffle_buff), std::end(shuffle_buff), entropy_source);

        // Copy the shuffled cards into the card list.
        std::copy(
            std::begin(shuffle_buff),
            std::end(shuffle_buff),
            std::front_inserter(m_card_list)
        );

    }

    friend std::ostream& operator<<(std::ostream& out, const Deck& code)
    {
        out << "[ ";
        for (const Card card : code.m_card_list) {
            out << card << ", ";
        }
        out << ']';

        return out;
    }


    iterator begin() { return m_card_list.begin(); }

    const_iterator begin() const { return m_card_list.begin(); }

    iterator end() { return m_card_list.end(); }

    const_iterator end() const { return m_card_list.end(); }

  private:

    /**
     * Helper function for producing a random PRNG seed using an available
     * hardware device.
     *
     * @return PRNG seed.
     */
    [[nodiscard]]
    static std::random_device::result_type default_random_seed()
    {
        // Handle for random seed generator using available hardware devices.
        // This technique for acquiring PRNG seeds is more secure than using
        // e.g. the current system time. This variable is given static duration
        // so that only one random device will be produced for this class.
        static std::random_device random_device{};
        // Return a freshly generated PRNG seed.
        return random_device();
    }

};

#endif //EECE_2560_PROJECTS_DECK_H
