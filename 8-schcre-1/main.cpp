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

#include <iostream>
#include <random>

#include "code.h"

int main()
{

//    std::random_device random_device{};
//    // Use implementation defined random engine.
//    std::default_random_engine entropy_source(random_device());

    Code test_cost(5, 9);
    Code test_guess(5, 9);
    std::cout << test_cost << '\n'
        << test_guess << '\n'
        << "Correct: " << test_cost.checkCorrect(test_guess)
        << ", Incorrect: " << test_cost.checkIncorrect(test_guess);




    // Test cases
    // 0 1 2 3 4 vs 1 2 3 4 0
    // [ 4 4 6 4 7 ] vs [ 8 1 5 5 6 ] => 0,1

}
