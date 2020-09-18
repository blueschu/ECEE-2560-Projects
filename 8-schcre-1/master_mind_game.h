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
 */

#ifndef ECEE_2560_PROJECTS_MASTER_MIND_GAME_H
#define ECEE_2560_PROJECTS_MASTER_MIND_GAME_H

#include "code.h"

#include <iostream>         // for I/O definitions

/**
 * A game of MasterMind with a fixed secret code. Implemented for part b.
 */
class MasterMindGame {

    /// The default number of digits in a secret code.
    constexpr inline static std::size_t DEFAULT_CODE_SIZE{5};

    /// The default radix for secret code digits.
    constexpr inline static unsigned int DEFAULT_DIGIT_RADIX{10};

    /// The number of incorrect guesses a player can make before losing the game.
    constexpr inline static unsigned int MAX_GUESSES{10};

    /// The number of digits in this game's secret code.
    [[maybe_unused]]
    const std::size_t m_code_size;

    /// This game's secret code.
    const Code m_secret_code;

  public:
    /**
     * Creates a mastermind game with an n-digit secret code in radix r, where
     * n = `code_size` and r = `digit-range`.
     *
     * @param code_size The number of digit in the secret code.
     * @param digit_range The radix of the secret code digits.
     */
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
