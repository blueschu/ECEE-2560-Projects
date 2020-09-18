/**
 * Mastermind game class declarations.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          David Cree      <cree.d@northeastern.edu>
 * Date:    2020-09-17
 *
 * References
 * ==========
 *
 *  - https://en.cppreference.com/w/cpp/language/aggregate_initialization
 *  - https://stackoverflow.com/questions/4178175/what-are-aggregates-and-pods-and-how-why-are-they-special
 */

#ifndef ECEE_2560_PROJECTS_MASTER_MIND_GAME_H
#define ECEE_2560_PROJECTS_MASTER_MIND_GAME_H

#include "code.h"

#include <iostream>         // for I/O definitions



class MasterMindGame {

    constexpr inline static std::size_t DEFAULT_CODE_SIZE{5};

    constexpr inline static unsigned int DEFAULT_DIGIT_RADIX{10};

    constexpr inline static  unsigned int MAX_GUESSES{10};

    [[maybe_unused]]
    const std::size_t m_code_size;

    const Code m_secret_code;

  public:
    explicit MasterMindGame(
        std::size_t code_size = DEFAULT_CODE_SIZE,
        unsigned int digit_range = DEFAULT_DIGIT_RADIX
    ) : m_code_size{code_size}, m_secret_code(code_size, digit_range) {}

    void run_game() const
    {
        // TODO
    }

};

#endif //ECEE_2560_PROJECTS_MASTER_MIND_GAME_H
