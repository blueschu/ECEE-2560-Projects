/**
 * Dictionary definitions for project 3.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          Chandler Cree   <cree.d@northeastern.edu>
 * Date:    2020-10-22
 *
 * References
 * ==========
 *  [1] https://en.cppreference.com/w/cpp/iterator/istream_iterator
 *
 */

#include "dictionary.h"

#include <algorithm>        // for std::transform
#include <cctype>           // for std::tolower
#include <fstream>          // for std::ifstream
#include <iterator>         // for std::istream_iterator

#include "eece2560_io.h"
#include "algo_util.h"

Dictionary Dictionary::read_file(const char* file_name)
{
    // todo error handling
    std::ifstream in_stream(file_name);

    std::vector<std::string> words;

    std::copy(
        std::istream_iterator<std::string>(in_stream),
        std::istream_iterator<std::string>(),
        std::back_inserter(words)
    );

    return Dictionary(std::move(words));
}

void Dictionary::sort_words()
{
    eece2560::selection_sort(std::begin(m_words), std::end(m_words));
}

std::ostream& operator<<(std::ostream& out, const Dictionary& dictionary)
{
    eece2560::print_sequence(out, std::begin(dictionary.m_words), std::end(dictionary.m_words));
    return out;
}

void Dictionary::normalize_word()
{
    for (auto& word : m_words) {
        std::transform(std::begin(word), std::end(word), std::begin(word),
            [](char c) { return std::tolower(c); }
        );
    }
}

bool Dictionary::contains(const std::string_view key) const
{
    const auto result = eece2560::binary_search(std::begin(m_words), std::end(m_words), key);
    return static_cast<bool>(result);
}
