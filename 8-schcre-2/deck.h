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
 *  [3]: https://en.cppreference.com/w/cpp/iterator/front_inserter
 */

#ifndef EECE_2560_PROJECTS_DECK_H
#define EECE_2560_PROJECTS_DECK_H

#include <algorithm>            // for std::random_shuffle
#include <forward_list>         // temporary: for standard list container
#include <optional>             // for std::optional
#include <ostream>              // for output stream definitions (iosfwd not sufficient)
#include <random>               // for random number generation
#include <vector>               // for std::vector (used in shuffle implementation)

#include "card.h"
#include "linked_list.h"

/**
 * A deck of playing cards.
 *
 * @tparam List The linked list type used to store the deck's cards.
 */
template<typename List = LinkedList<Card>>
class Deck {

    /// The number of playing cards in a full deck.
    constexpr static std::size_t FULL_DECK_SIZE{Card::ALL_RANKS.size() * Card::ALL_SUITS.size()};

    /// This deck's list of cards
    List m_card_list;

  public:
    using iterator = typename List::iterator;
//    using const_iterator = typename List::const_iterator;

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

    std::optional<Card> deal()
    {
        if (m_card_list.empty()) {
            return std::nullopt;
        }
        auto front = m_card_list.front();
        m_card_list.pop_front();
        return front;
    }

    friend std::ostream& operator<<(std::ostream& out, const Deck& code)
    {
        out << "[ ";
        // TODO remove const cast after const iterators are implemented
        for (const Card card : const_cast<LinkedList<Card>&>(code.m_card_list)) {
            out << card << ", ";
        }
        out << ']';

        return out;
    }

    /*
     * Range member functions to allow Decks to be used as iterable ranges.
     */

    iterator begin() { return m_card_list.begin(); }

//    const_iterator begin() const { return m_card_list.begin(); }

    iterator end() { return m_card_list.end(); }

//    const_iterator end() const { return m_card_list.end(); }

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
