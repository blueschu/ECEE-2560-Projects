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

#include <functional>       // for std::function

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
    const std::size_t m_code_size;

    /// This game's secret code.
    const Code m_secret_code;

  public:
    /**
     * Callable type for generating guesses during a mastermind game.
     */
    using GuessGenerator = std::function<Code()>;

    /**
     * Callable type for displaying the number of guesses remaining and the
     * result of a guess during a mastermind game.
     */
    using ResponseCallback = std::function<void(int, GuessResponse)>;

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

    /**
     * Returns this game's secret code.
     *
     * @return Ths game's secret code.
     */
    [[nodiscard]]
    const Code& get_code() const noexcept { return m_secret_code; }

    /**
     * Runs a game of mastermind using this game's secret code.
     *
     * This function accepts a `guess_generator` function object for producing
     * user guesses during the same.
     *
     * @param guess_generator Callable for generating user guesses.
     * @param response_callback Callback for displaying use results to the user.
     * @return true if the user one the game, false if they lost.
     */
    bool run_game(
        const GuessGenerator& guess_generator,
        const ResponseCallback& response_callback) const
    {
        int guesses_remaining{MAX_GUESSES};

        while (guesses_remaining > 0) {
            const Code guess = guess_generator();
            const GuessResponse result = m_secret_code.check_guess(guess);

            if (check_solution(result)) {
                return true;
            }

            --guesses_remaining;
            response_callback(guesses_remaining, result);
        } // end while

        // The user did not guess the code before reaching the guess limit.
        return false;
    }

  private:
    /**
     * Returns true if the given guess response indicates that the user won
     * the game.
     *
     * @param guess_response Response to the user's guess.
     * @return True if the response indicates that the user won the game.
     */
    [[nodiscard]]
    bool check_solution(const GuessResponse& guess_response) const noexcept
    {
        return guess_response.correct_count == m_code_size;
    }

};

#endif //ECEE_2560_PROJECTS_MASTER_MIND_GAME_H
