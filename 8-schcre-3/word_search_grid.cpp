/**
 * Grid definitions for project 3.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          Chandler Cree   <cree.d@northeastern.edu>
 * Date:    2020-10-25
 *
 */

#include "word_search_grid.h"

#include <fstream>          // for std::ifstream
#include <iterator>         // for std::istream_iterator
#include <vector>           // for std::vector

WordSearchGrid WordSearchGrid::read_file(const char* file_name)
{
    std::ifstream in_stream(file_name);
    in_stream.exceptions(std::ios::badbit);

    if (!in_stream) {
        throw std::runtime_error("word search file does not exist");
    }

    std::size_t rows;
    std::size_t cols;

    // Exception raised on invalid input since badbit is set.
    in_stream >> rows >> cols >> std::ws;

    std::vector<Entry> grid_letters;

    std::copy(
        std::istream_iterator<Entry>(in_stream),
        std::istream_iterator<Entry>(),
        std::back_inserter(grid_letters)
    );

    Matrix<Entry> mat(std::move(grid_letters));
    mat.reshape({rows, cols});
    return WordSearchGrid(std::move(mat));

}
