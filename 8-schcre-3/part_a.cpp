/**
 * Project 3a main file.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          Chandler Cree   <cree.d@northeastern.edu>
 * Date:    2020-10-22
 *
 */

#include <iostream>             // for I/O stream definitions
#include <numeric>              // for std::iota

#include "eece2560_io.h"
#include "dictionary.h"
#include "ordinal_wrapping_sequence.h"
#include "matrix.h"

constexpr const char* DICTIONARY_FILE = "resources/dictionary.txt";

int main()
{
    // Temporary tests

    const auto dictionary = Dictionary::read_file(DICTIONARY_FILE);
    std::cout << "Dictionary: " << dictionary << '\n';

    constexpr std::size_t dim{3};

    std::vector<int> values(dim*dim);
    std::iota(std::begin(values), std::end(values), 0);

    Matrix<int> m{std::move(values)};
    m.reshape({3,3});

    eece2560::print_sequence(std::cout, std::begin(m), std::end(m));
    std::cout << '\n';

    OrdinalWrappingSequenceIter<int> it{m};
    OrdinalWrappingSequenceIter<int> end{};

    while (it != end) {
        eece2560::print_sequence(std::cout, std::begin(*it), std::end(*it));
        ++it;
        std::cout << '\n';
    }
}
