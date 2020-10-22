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

#include <fstream>          // for std::ifstream
#include <iterator>         // for std::istream_iterator

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

}
