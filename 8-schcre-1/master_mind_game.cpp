/**
 * Mastermind game definitions for project 1.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          Chandler Cree   <cree.d@northeastern.edu>
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
        const GuessResponse result = compute_guess_response(
            generate_user_guess(guess_generator)
        );

        if (check_solution(result)) {
            return true;
        }

        --guesses_remaining;
        response_callback(guesses_remaining, result);
    } // end while

    // The user did not guess the code before reaching the guess limit.
    return false;
}
