/**
 * Project 1 main file.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          David Cree      <cree.d@northeastern.edu>
 * Date:    2020-09-17
 *
 * References
 * ==========
 *
 *  - https://en.cppreference.com/w/cpp/numeric/random
 */

#include <iostream>         // for std::cout
#include <string_view>      // for std::string_view

#include "code.h"

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

} // end namespace

int main()
{
    const auto code_width = prompt_user<std::size_t>("Please enter a code size: ");
    const auto digit_range = prompt_user<unsigned int>("Please enter a code radix: ");

    const Code code(code_width, digit_range);

    std::cout << "Secret code:    " << code << '\n';

    // Only run demo tests for 5-digit codes.
    if (code_width == 5) {
        const auto demo_cases = std::array{
            Code{5, 0, 3, 2, 6}, Code{2, 1, 2, 2, 2}, Code{1, 3, 3, 4, 5}
        };

        for (const Code& guess : demo_cases) {
            const auto result = code.check_guess(guess);
            std::cout << "Result of guess " << guess << ": "
                      << result.correct_count << ","
                      << result.incorrect_count << '\n';
        }
    }

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
}
