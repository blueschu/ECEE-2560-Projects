/**
 * Project 3b main file.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          Chandler Cree   <cree.d@northeastern.edu>
 * Date:    2020-10-22
 *
 */

#include <algorithm>
#include <complex>
#include <iostream>
#include <vector>

#include "algo_util.h"
#include "eece2560_io.h"


int main()
{
    using namespace std::complex_literals;

    constexpr auto comp_real = [=](auto lhs, auto rhs) {
        return std::real(lhs) < std::real(rhs);
    };
    std::vector<std::complex<double>> fred{
        5.0+0i, 6, 2.0+0i, 89, 1, 2.0+1i, -2.0+0i, 3, -2.0+1i, -2.0+2i, 67, 8, 4, 5.0+1i, 5.0+2i
    };
//    std::vector<std::complex<double>> fred{
//        2, -2.0+0i, 3, -2.0+1i, -2.0+2i, 67, 8, 4, 5.0+1i, 5.0+2i
//    };

//    auto it = eece2560::details::partition_unstable(std::begin(fred), std::end(fred), comp_real);
//    std::cout << *it << '\n';
    eece2560::quicksort_unstable(std::begin(fred), std::end(fred), comp_real);

    eece2560::print_sequence(std::cout, std::begin(fred), std::end(fred));
}
