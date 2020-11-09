/**
 * Project 3b main file.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          Chandler Cree   <cree.d@northeastern.edu>
 * Date:    2020-11-06
 *
 */

#include <iostream>             // for I/O stream definitions

#include "algo_util.h"
#include "eece2560_io.h"
#include "eece2560_iter.h"
#include "dictionary.h"
#include "word_search_grid.h"

/// The minimum length of valid words in the word search grid.
constexpr std::size_t MIN_WORD_LENGTH{5};

constexpr const char* DICTIONARY_FILE = "resources/dictionary.txt";

/**
 * Prints all words contained in the given dictionary that appear in the given
 * word search grid.
 *
 * @param dictionary Dictionary of valid words.
 * @param grid Word search grid.
 */
void print_matches(const Dictionary& dictionary, const WordSearchGrid& grid)
{
    constexpr static auto filter_words = [](auto word) {
        return word.size() >= MIN_WORD_LENGTH;
    };

    eece2560::FilterIter start(std::begin(grid), std::end(grid), filter_words);
    eece2560::FilterIter end(std::end(grid), std::end(grid), filter_words);

    std::size_t found_count{0};

    while (start != end) {
        std::string_view key{start->data(), start->size()};

        if (dictionary.contains(key)) {
            ++found_count;
            std::cout << "Found: " << key << '\n';
        }
        ++start;
    }
    std::cout << "\nFound " << found_count << " words.\n";

}

/**
 * Prompts the user for a file containing a word search and prints all words
 * contained in word search.
 */
void run_word_search(Dictionary::SortingAlgorithm algorithm)
{
    std::cout << "Preparing the dictionary . . . " << std::flush;
    const auto dictionary = Dictionary::read_file(DICTIONARY_FILE, algorithm);
    std::cout << "DONE\nDictionary: " << dictionary << '\n';

    const auto word_search_file = eece2560::prompt_user<std::string>(
        "Enter the word search file name (e.g. \"resources/15x15.txt\"): "
    );

    const auto grid = WordSearchGrid::read_file(word_search_file.c_str());

    print_matches(dictionary, grid);
}

int main()
{
    auto sorting_algorithm = eece2560::prompt_user<Dictionary::SortingAlgorithm>(
        "Pick the dictionary sorting algorithm (0 for selection sort, 1 for quick sort, 2 for heap sort): "
    );
    std::cout << "Using " << sorting_algorithm << '\n';
    run_word_search(sorting_algorithm);
}
