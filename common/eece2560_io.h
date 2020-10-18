/**
 * Common I/O utilities for EECE 2560 projects.
 *
 * For ease of user, these utilities are implemented as a header-only library.
 *
 * References:
 * ===========
 *  [1] https://en.cppreference.com/w/cpp/iterator/advance
 *  [2] https://en.cppreference.com/w/cpp/iterator
 */

#ifndef EECE_2560_PROJECTS_EECE2560_IO_H
#define EECE_2560_PROJECTS_EECE2560_IO_H

#include <functional>            // for std::function
#include <iostream>             // for I/O definitions (iosfwd not sufficient)
#include <iterator>             // for std::iterator_traits
#include <string_view>          // for std::string_view
#include <type_traits>          // for std::is_base_of

namespace eece2560 {

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
T prompt_user(std::string_view prompt)
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

/**
 * Returns true if the given string represents an affirmative statement.
 *
 * Used for determining whether a user's input for a boolean value should be
 * interpreted as a `true`.
 *
 * @param response User's response to a yes/no question.
 * @return True is the response is affirmative.
 */
constexpr inline bool is_affirmation(std::string_view response) noexcept
{
    using namespace std::string_view_literals;
    constexpr auto affirm_statements = std::array{
        "y"sv, "yes"sv, "t"sv, "true"sv, "1"sv
    };
    for (auto s : affirm_statements) {
        if (s == response) {
            return true;
        }
    }
    return false;
}

/**
 * Returns true if the given string represents a "negative" statement.
 *
 * Used for determining whether a user's input for a boolean value should be
 * interpreted as a `false`.
 *
 * @param response User's response to a yes/no question.
 * @return True is the response is a negation.
 */
constexpr inline bool is_negation(std::string_view response) noexcept
{
    using namespace std::string_view_literals;
    constexpr auto negation_statements = std::array{
        "n"sv, "no"sv, "f"sv, "false"sv, "0"sv
    };
    for (auto s : negation_statements) {
        if (s == response) {
            return true;
        }
    }
    return false;
}

inline bool prompt_user(
    std::string_view prompt,
    std::function<bool(std::string_view)> is_affirm,
    std::function<bool(std::string_view)> is_negate
)
{
    std::string response;
    while (true) {
        std::cout << prompt;
        // For simplicity we assume that reading a string will not fail.
        std::cin >> response;
        if (is_affirm(response)) {
            return true;
        }

        if (is_negate(response)) {
            return false;
        }

        // Print and immediately flush error message.
        std::cout << "Invalid input" << std::endl;

        // No need to manipulate stream error state.
    }
}

template<>
inline bool prompt_user(std::string_view prompt)
{
    return prompt_user(prompt, is_affirmation, is_negation);
}

/**
 * Prints the range [it, end) to given output stream with elements separated by
 * the given delimiter and with the entire sequence enclosed by the given
 * open and close symbols.
 *
 * @tparam Iter Iterator type. May be any input iterator.
 * @param out Output stream to be written to.
 * @param it,end Range to be printed.
 * @param delim String to be printed between internal elements of the range.
 * @param open_symbol String to be printed at the beginning of the list.
 * @param close_symbol String to be printed at the end of the list.
 */
template<typename Iter>
void print_list(
    std::ostream& out,
    Iter it,
    Iter end,
    std::string_view delim = ", ",
    std::string_view open_symbol = "[",
    std::string_view close_symbol = "]")
{
    using category = typename std::iterator_traits<Iter>::iterator_category;
    static_assert(std::is_base_of_v<std::input_iterator_tag, category>);

    out << open_symbol;

    // For random access and bidirectional iterators, we can directly compute
    // the iterator to the last element.
    if constexpr (std::is_base_of_v<std::bidirectional_iterator_tag, category>) {
        // Iterator to the last element.
        auto before_end = end;
        --before_end;

        // Print internal elements separated by the given delimiter.
        while (it != before_end) {
            out << *it << delim;
            ++it;
        }
        // Print the last element with no trailing delimiter.
        out << *before_end;

    } else {    // For forward iterators.
        // Track the previous iterator during iteration. This way when the
        // end iterator is reached, we still have a reference to the last
        // element, which we can print without a trailing delimiter.
        auto prev = it++;

        // Print internal elements separated by the given delimiter.
        while (it != end) {
            out << *prev << delim;
            prev = it++;
        }
        // Print the last element with no trailing delimiter.
        out << *prev;
    }

    out << close_symbol;
}

} // end namespace eece2560


#endif //EECE_2560_PROJECTS_EECE2560_IO_H
