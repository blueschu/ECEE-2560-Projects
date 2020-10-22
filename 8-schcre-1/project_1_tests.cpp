/**
 * Test executable for Project 1.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          Chandler Cree   <cree.d@northeastern.edu>
 * Date:    2020-09-17
 *
 */


#include <iostream>
#include <utility>
#include <vector>

#include "code.h"

// Using anonymous namespace to give symbols internal linkage.
namespace {
struct CodeTestCase {
    Code code;
    Code guess;
    GuessResponse response;
};
} // end namespace

int main()
{
    const auto test_cases = std::array{
        CodeTestCase{{1, 2, 3}, {1, 2, 3}, {3, 0}},
        CodeTestCase{{1, 3, 2}, {1, 2, 3}, {1, 2}},
        CodeTestCase{{0, 1, 2, 3, 4}, {1, 2, 3, 4, 0}, {0, 5}},
        CodeTestCase{{3, 2, 3, 3, 3}, {1, 3, 3, 4, 5}, {1, 1}},
        CodeTestCase{{1, 2, 3, 4, 5}, {5, 0, 3, 2, 6}, {1, 2}},
        CodeTestCase{{1, 2, 3, 4, 5}, {4, 4, 3, 3, 3}, {1, 1}},
        CodeTestCase{{0, 2, 0, 2, 0}, {1, 2, 0, 4, 2}, {2, 1}}
    };

    std::size_t case_number{0};

    for (const CodeTestCase& test_case : test_cases) {
        const auto result = test_case.code.check_guess(test_case.guess);

        if (result == test_case.response) {
            std::cout << "Case " << case_number << " OK\n";
        } else {
            std::cout << "Case " << case_number << " FAILED:\n"
                      << "Code:  " << test_case.code << '\n'
                      << "Guess: " << test_case.guess << '\n'
                      << "Expected (C,I): " << test_case.response << '\n'
                      << "Actual (C,I):   " << result << '\n';
        }
        ++case_number;
    }

}
