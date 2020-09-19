/**
 * Project 1b main file.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          David Cree      <cree.d@northeastern.edu>
 * Date:    2020-09-19
 *
 * References
 * ==========
 *
 *  - https://en.cppreference.com/w/cpp/header/type_traits
 *  - https://en.cppreference.com/w/cpp/io/basic_istringstream
 *  - https://en.cppreference.com/w/cpp/algorithm/copy
 *  - https://en.cppreference.com/w/cpp/iterator/back_inserter
 */

#include <algorithm>        // for std::copy
#include <iostream>         // for I/O definitions
#include <iterator>         // for std::istream_iterator, std::back_inserter
#include <sstream>          // for string streams
#include <string_view>      // for std::string_view

#include "code.h"
#include "master_mind_game.h"

// Using anonymous namespace to give symbols internal linkage.
namespace {
/**
 * Prints the specified prompt to the standard output and reads a T value
 * from the standard input.
 *
 * If the user provides an invalid T value, the standard input will be cleared
 * and the user will be reprompted.
 *
 * @tparam T The type of the object to read.
 * @param prompt The message to be displayed to the user.
 * @return User provided T value.
 */
template<typename T>
T prompt_user(std::string_view prompt);

/**
 * Prints a prompt to the standard input and read a Code value from the standard
 * input.
 *
 * If the user provides a guess that does not consist of `code_size` integers,
 * the they will be prompted for a new input.
 *
 * @param code_size The number of digits in a valid code guess.
 * @return User provided code.
 */
Code prompt_user_guess(std::size_t code_size);

/**
 * Displays the result of a user's guess to the standard output.
 *
 * @param guesses_remaining The number of guess remaining or a sentinel
 *                          indicating that the user won the game.
 * @param result The result of the user's guess.
 */
void display_guess_result(int guesses_remaining, GuessResponse result);

static_assert(
    std::is_constructible_v<MasterMindGame::ResponseCallback, decltype(display_guess_result)>,
    "display_guess_result is not a valid mastermind game callback"
);

} // end namespace

int main()
{
    const auto code_size = prompt_user<std::size_t>("Please enter a code size: ");
    const auto digit_range = prompt_user<unsigned int>("Please enter a code radix: ");

    // Empty the input stream until a newline character is found.
    // We later use std::getline to acquire user input, so we need to remove
    // trailing newline left in the input buffer from using "std::cin >> var".
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    const MasterMindGame master_mind_game(code_size, digit_range);

    std::cout << "Secret code: " << master_mind_game.get_code() << '\n';

    master_mind_game.run_game(
        [=]() { return prompt_user_guess(code_size); },
        display_guess_result
    );

}

namespace {
template<typename T>
T prompt_user(const std::string_view prompt)
{
    T user_selection;
    while (true) {
        std::cout << prompt;
        // operator>>(std::istream&, T) returns false on input failure.
        // We assume EOF will not be reached.
        if (!(std::cin >> user_selection)) {
            // Print and immediately flush error message.
            std::cout << "Invalid input" << std::endl;
            // Clear the error
            std::cin.clear();
            // Empty the input stream until a newline character is found.
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        return user_selection;
    }
}

Code prompt_user_guess(std::size_t code_size)
{
    std::vector<Code::Digit> digits;

    while (true) {
        std::cout << "Enter a guess: ";
        std::string user_input;
        // Fetch a complete line of input from the user.
        std::getline(std::cin, user_input);

        // Wrap the user-provided stream in stream so that input stream
        // operations can be performed on it (e.g., reading integers).
        std::istringstream input_stream(user_input);

        // Repeatedly read integers from the user-provided string until it is
        // depleted. Note that we do not want to use std::istream_iterator<Code::Digit>
        // here, since that may lead to digits being ASCII encoded.
        std::copy(
            std::istream_iterator<int>(input_stream),
            std::istream_iterator<int>(),
            std::back_inserter(digits)
        );

        if (digits.size() != code_size) {
            std::cout << "Invalid input. Code guess must consist of "
                      << code_size << " digits\n";
            // Empty the zero vector without de-allocating it storage.
            digits.clear();
            continue;
        }

        // Display the guess to the user.
        Code code(std::move(digits));
        std::cout << "Guess: " << code << '\n';
        return code;
    }
}

void display_guess_result(int guesses_remaining, GuessResponse result)
{
    if (guesses_remaining == MasterMindGame::WON_SENTINEL) {
        std::cout << "You guess the code!\n";
    } else {
        using namespace std::string_view_literals;
        const auto guess_plural = (guesses_remaining == 1) ? " guess"sv : " guesses"sv;

        std::cout << "Result: " << result
                  << "\nYou have " << guesses_remaining << guess_plural << " remaining\n";
    }
}
}
