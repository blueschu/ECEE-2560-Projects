/**
 * Grid definitions for project 3.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          Chandler Cree   <cree.d@northeastern.edu>
 * Date:    2020-10-25
 *
 */

#include "word_search_grid.h"

#include <fstream>
#include <iterator>

#include "eece2560_io.h"
#include "algo_util.h"


Grid Grid::read_chars(const char* file_name)
{
    std::ifstream in_stream(file_name);

    std::vector<char> chars_grid;

    std::copy(
        std::istream_iterator<char>(in_stream),
        std::istream_iterator<char>(),
        std::back_inserter(chars_grid)
    );

    return chars_grid; //incorrect return
}
