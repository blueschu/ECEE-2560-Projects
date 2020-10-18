/**
 * Common I/O utilities used in project 2 and beyond.
 *
 * For ease of user, these utilities are implemented as a header-only library.
 *
 * References:
 * ===========
 *  [1] https://en.cppreference.com/w/cpp/iterator/advance
 *  [2] https://en.cppreference.com/w/cpp/iterator
 *  [3] https://en.cppreference.com/w/cpp/string/basic_string/getline
 */

#ifndef EECE_2560_PROJECTS_EECE2560_IO_H
#define EECE_2560_PROJECTS_EECE2560_IO_H

#include <functional>           // for std::function
#include <iostream>             // for I/O definitions (iosfwd not sufficient)
#include <iterator>             // for std::iterator_traits
#include <sstream>              // for std::istringstream
#include <string_view>          // for std::string_view
#include <type_traits>          // for std::is_base_of

namespace eece2560 {

/**
 * Functor for converting a string into a T instance using T's stream extraction
 * operator overload.
 *
 * @tparam T The type to be extracted from a string.
 */
template<typename T>
struct StreamExtractor {
    std::optional<T> operator()(const std::string& line) const
    {
        // Wrap the user-provided line in a stream so that input stream
        // operations can be performed on it.
        std::istringstream stream(line);
        T temp;
        stream >> temp;
        if (!stream) {
            return std::nullopt;
        } else {
            return temp;
        }
    }
};

/**
 * Functor for extracting a boolean value from a string.
 *
 * @tparam AffirmPred Callable type for determining whether the input is truthy,
 * @tparam NegatePred Callable type for determining whether the input is fasley.
 */
template<typename AffirmPred, typename NegatePred>
struct BoolAlphaExtractor {

    /**
     * Predicate function object for determining whether a string represents
     * the affirmation of a prompt; i.e., a truthy statement.
     */
    AffirmPred m_is_affirmation;

    /**
     * Predicate function object for determining whether a string represents
     * the negation of a prompt; i.e., a falsey statement.
     */
    NegatePred m_is_negation;

    /**
     * Constructs a BoolAlphaExtractor with the given affirmation and negation
     * predicates.
     * @param is_affirm Callable that returns true for truthy values.
     * @param is_negate Callable that returns true for falsey values.
     */
    constexpr BoolAlphaExtractor(AffirmPred is_affirm, NegatePred is_negate) noexcept
        : m_is_affirmation(is_affirm), m_is_negation(is_negate) {}

    std::optional<bool> operator()(std::string_view line) const
    {
        if (m_is_affirmation(line)) {
            return true;
        }
        if (m_is_negation(line)) {
            return false;
        }
        return std::nullopt;
    }
};


/**
 * Prints the specified prompt to the standard output and attempts to produce
 * a T value from a complete line of input read from the standard input.
 *
 * The mapping between input lines and T values is specified by the
 * try_from_str parameter, which should be a callable that returns a
 * std::optional<T>.
 *
 * If try_from_str returns a std::nullopt, the user's input is considered to be
 * an invalid T value and the user will be reprompted.
 *
 * If try_from_str returns a wrapped T value, that value will be returned.
 *
 * By default, try_from_str with use T' stream extraction operator overload
 * to attempt to read a T value from a string stream containing the input line.
 *
 * @tparam T The type of the object to read.
 * @tparam FromStr Functor mapping strings to T's.
 * @param prompt The message to be displayed to the user.
 * @return User provided T value.
 */
template<typename T, typename FromStr = StreamExtractor<T>>
T prompt_user(std::string_view prompt, FromStr try_from_str = FromStr())
{
    std::string line;
    while (true) {
        std::cout << prompt;
        // For simplicity we assume that reading a string will not fail.
        std::getline(std::cin, line);

        // Most likely a std::optional, but we do not restrict implementors
        // from using a custom option-like class instead.
        auto parsed_value = try_from_str(line);

        if (parsed_value) {
            return *parsed_value;
        } else {
            std::cout << "Invalid input" << std::endl;
            continue;
        }
    }
}

/**
 * Returns true if the given string represents an affirmative statement
 * according to a default whitelist.
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
 * Returns true if the given string represents a negation statement
 * according to a default whitelist.
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

/**
 * BoolAlphaExtractor instance using the default whitelists for prompt affirmations
 * and negations.
 */
inline constexpr BoolAlphaExtractor bool_alpha_extractor{is_affirmation, is_negation};


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
