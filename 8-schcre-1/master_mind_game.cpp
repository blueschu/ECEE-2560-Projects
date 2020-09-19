/**
 * Mastermind game definitions.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          David Cree      <cree.d@northeastern.edu>
 * Date:    2020-09-17
 *
 */

#include "master_mind_game.h"

bool MasterMindGame::run_game(
    const MasterMindGame::GuessGenerator& guess_generator,
    const MasterMindGame::ResponseCallback& response_callback
) const
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
