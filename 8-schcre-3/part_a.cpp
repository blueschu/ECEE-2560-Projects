/**
 * Project 3a main file.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          Chandler Cree   <cree.d@northeastern.edu>
 * Date:    2020-10-22
 *
 */

#include <iostream>             // for I/O stream definitions

#include "algo_util.h"
#include "eece2560_iter.h"
#include "dictionary.h"
#include "word_search_grid.h"

[[maybe_unused]]
constexpr const char* DICTIONARY_FILE = "resources/dictionary.txt";
[[maybe_unused]]
constexpr const char* WORD_SEARCH_FILE = "resources/15x15.txt";

int main()
{
    // Temporary tests

    const auto dictionary = Dictionary::read_file(DICTIONARY_FILE);
    std::cout << "Dictionary: " << dictionary << '\n';

    const auto grid = Grid::read_file(WORD_SEARCH_FILE);

    const auto filter_words = [](auto word) {
        return word.size() >= 5;
    };

    eece2560::FilterIter start(std::begin(grid), std::end(grid), filter_words) ;
    eece2560::FilterIter end(std::end(grid), std::end(grid), filter_words) ;

    while (start != end) {
        std::string_view key{start->data(), start->size()};

        if (dictionary.contains(key)) {
            std::cout << "Found:" << key << '\n';
        }
        ++start;
    }
}
