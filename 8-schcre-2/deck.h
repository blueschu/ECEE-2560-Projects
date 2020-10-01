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
 *  [2]: https://en.cppreference.com/w/cpp/container/forward_list
 *  [3]: https://en.cppreference.com/w/cpp/iterator/front_inserter (no longer used)
 */

#ifndef EECE_2560_PROJECTS_DECK_H
#define EECE_2560_PROJECTS_DECK_H

// If defined, Deck will use the C++ standard libary's std::forward_list as its
// internal linked list. Otherwise, the custom LinkedList class will be used.
//#define USE_STANDARD_LIST

#include <algorithm>            // for std::random_shuffle
#include <optional>             // for std::optional
#include <ostream>              // for output stream definitions (iosfwd not sufficient)
#include <random>               // for random number generation
#include <vector>               // for std::vector (used in shuffle implementation)

#include "card.h"

#ifdef USE_STANDARD_LIST
#include <forward_list>         // temporary: for standard list container
using DefaultList = std::forward_list<Card>;
#else
#include "linked_list.h"
using DefaultList = LinkedList<Card>;
#endif

/**
 * A deck of playing cards.
 *
 * @tparam List The linked list type used to store the deck's cards.
 */
template<typename List = DefaultList>
class Deck {

    /// The number of playing cards in a full deck.
    constexpr static std::size_t FULL_DECK_SIZE{Card::ALL_RANKS.size() * Card::ALL_SUITS.size()};

    /// This deck's list of cards
    List m_card_list;

  public:
    // Forward iterator type used by the internal playing card list.
    using iterator = typename List::iterator;
    // Forward const iterator type used by the internal playing card list.
    using const_iterator = typename List::const_iterator;

    /**
     * Creates a Deck will all 52 cards in their sorted order.
     */
    Deck()
    {
        // Iterator to before the start of the card list.
        auto it = m_card_list.before_begin();

        for (auto rank : Card::ALL_RANKS) {
            for (auto suit : Card::ALL_SUITS) {
                // Insert the card with the current rank and suite at the end
                // of the list. Update the iterator to the end of the list.
                it = m_card_list.insert_after(it, {rank, suit});
            }
        }
    }

    template<typename R = std::default_random_engine>
    void shuffle(R entropy_source = R(default_random_seed()))
    {
        // std::shuffle requires a random access iterator, so we copy the card
        // list into a random access container. We cannot use a fixed size array
        // since we do not provide a default constructor for Card.
        std::vector<Card> shuffle_buff(std::begin(m_card_list), std::end(m_card_list));

        std::shuffle(std::begin(shuffle_buff), std::end(shuffle_buff), entropy_source);

        // Create a new card list from the shuffle cards.
        List new_list(std::begin(shuffle_buff), std::end(shuffle_buff));

        // Move the new list into this deck's card list. The old card list will
        // be automatically dropped when new_list goes out of scope.
        m_card_list = std::move(new_list);
    }

    std::optional<Card> deal()
    {
        if (m_card_list.empty()) {
            return std::nullopt;
        }
        auto front = m_card_list.front();
        m_card_list.pop_front();
        return front;
    }

    friend std::ostream& operator<<(std::ostream& out, const Deck<List>& deck)
    {
        out << "[ ";
        for (const Card card : deck) {
            out << card << ", ";
        }
        out << ']';

        return out;
    }

    /*
     * Range member functions to allow Decks to be used as iterable ranges.
     */

    iterator begin() { return m_card_list.begin(); }

    const_iterator begin() const { return m_card_list.begin(); }

    iterator end() { return m_card_list.end(); }

    const_iterator end() const { return m_card_list.end(); }

  private:

    /**
     * Helper function for producing a random PRNG seed using an available
     * hardware device.
     *
     * This function will yield a new PRNG seed each time it is called.
     *
     * @return new PRNG seed.
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
