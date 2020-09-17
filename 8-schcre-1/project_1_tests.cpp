//
// Created by brian on 9/17/20.
//


#include <iostream>
#include <utility>
#include <vector>

#include "code.h"

struct CodeTestCase {

    std::vector<Code::Digit> code;
    std::vector<Code::Digit> guess;
    std::size_t correct;
    std::size_t incorrect;

};

int main()
{
    const auto test_cases = std::array{
        CodeTestCase{{1, 2, 3}, {1, 2, 3}, 3, 0},
        CodeTestCase{{1, 3, 2}, {1, 2, 3}, 1, 2},
        CodeTestCase{{0, 1, 2, 3, 4}, {1, 2, 3, 4, 0}, 0, 5},
        CodeTestCase{{3, 2, 3, 3, 3}, {1, 3, 3, 4, 5}, 1, 1},
        CodeTestCase{{1, 2, 3, 4, 5}, {5, 0, 3, 2, 6}, 1, 2},
        CodeTestCase{{1, 2, 3, 4, 5}, {4, 4, 3, 3, 3}, 1, 1},
        CodeTestCase{{0, 2, 0, 2, 0}, {1, 2, 0, 4, 2}, 2, 1}
    };

    std::size_t case_number{0};

    for (const CodeTestCase& test_case : test_cases) {
        const Code code(test_case.code);
        const Code guess(test_case.guess);

        const auto correct = code.checkCorrect(guess);
        const auto incorrect = code.checkIncorrect(guess);

        if (correct == test_case.correct && incorrect == test_case.incorrect) {
            std::cout << "Case " << case_number << " OK\n";
        } else {
            std::cout << "Case " << case_number << " FAILED:\n"
                      << "Code:  " << code << '\n'
                      << "Guess: " << guess << '\n'
                      << "Expected C/I:" << test_case.correct << '/' << test_case.incorrect << '\n'
                      << "Actual C/I:  " << correct << '/' << incorrect << '\n';
        }
        ++case_number;
    }

}
