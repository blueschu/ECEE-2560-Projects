/**
 * Code class for Project 1.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          David Cree      <cree.d@northeastern.edu>
 * Date:    2020-09-17
 *
 * References
 * ==========
 *
 *  - https://en.cppreference.com/w/cpp/algorithm/set_intersection
 *  - https://en.cppreference.com/w/cpp/iterator/back_inserter
 *
 */

#include "code.h"

#include <iostream>

GuessResponse::Count Code::check_correct(const Code& guess) const
{
    if (m_digits.size() != guess.m_digits.size()) {
        throw MismatchedCodeLengthError("cannot compare Code instances of unequal lengths");
    }

    // The number of digits that match in both value and position.
    GuessResponse::Count match_count{0};
    // Iterator into the guess code.
    auto right_it = std::begin(guess.m_digits);

    // Simultaneously iterate over the digits in this code and in the guess
    // code. Count the number of positions where the digits match.
    for (const Digit left_digit : m_digits) {
        if (left_digit == *right_it) {
            ++match_count;
        }
        ++right_it;
    }

    return match_count;
}

GuessResponse::Count Code::check_incorrect(const Code& guess) const
{
    if (m_digits.size() != guess.m_digits.size()) {
        throw MismatchedCodeLengthError("cannot compare Code instances of unequal lengths");
    }

    // Sequence of digits from this code that do not match the guess digits
    // in both value and position. Relative ordering of digits is maintained.
    std::vector<Digit> differing_digits_left{};
    // Sequence of digits from the guess that do not match this code's digits
    // in both value and position. Relative ordering of digits is maintained.
    std::vector<Digit> differing_digits_right{};

    // Iterator into the guess code.
    auto right_it = std::begin(guess.m_digits);

    // Simultaneously iterate over the digits in this code and in the guess
    // code. Whenever the digits in this code and the guess code do not agree,
    // copy the digits into the "differing digits" vectors.
    // In essence, we filter away the "correct" digits. This is safe to do,
    // since no digit can be both "correct" and "incorrect".
    for (const Digit left_digit : m_digits) {
        if (left_digit != *right_it) {
            differing_digits_left.push_back(left_digit);
            differing_digits_right.push_back(*right_it);
        }
        ++right_it;
    }

    // Sort the differing digits to that they can be treated as multisets.
    std::sort(std::begin(differing_digits_left), std::end(differing_digits_left));
    std::sort(std::begin(differing_digits_right), std::end(differing_digits_right));

    std::vector<Digit> incorrect_digits{};

    // Extract the digits that are present in both "differing digits" sequences,
    // with repeats treated as unique values. These are precisely the values
    // that appear in both sequences, but which do not appear in the same
    // positions.
    std::set_intersection(
        std::begin(differing_digits_left),
        std::end(differing_digits_left),
        std::begin(differing_digits_right),
        std::end(differing_digits_right),
        std::back_inserter(incorrect_digits)
    );

    return static_cast<GuessResponse::Count>(incorrect_digits.size());

}

std::ostream& operator<<(std::ostream& out, const GuessResponse& guess_response)
{
    out << '(' << guess_response.correct_count << ',' << guess_response.incorrect_count << ')';
    return out;
}

std::ostream& operator<<(std::ostream& out, const Code& code)
{
    out << "[ ";
    for (Code::Digit digit : code.m_digits) {
        // Make sure that digits are not interpreted as encoded characters,
        // since we use an integral type with the same size as `char`.
        out << static_cast<int>(digit) << ' ';
    }
    out << ']';

    return out;
}
