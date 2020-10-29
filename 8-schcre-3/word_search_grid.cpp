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


Grid Grid::read_file(const char* file_name)
{
    // todo error handling
    std::ifstream in_stream(file_name);
//    in_stream.exceptions(std::ios::badbit | std::ios::failbit);

    if (!in_stream) {
        throw std::runtime_error("word search file does not exist");
    }

    std::size_t rows;
    std::size_t cols;

    if (!(in_stream >> rows >> cols >> std::ws)) {
        // todo error handling
    }

    std::vector<Entry> grid_letters;

    std::copy(
        std::istream_iterator<Entry>(in_stream),
        std::istream_iterator<Entry>(),
        std::back_inserter(grid_letters)
    );

    try {
        Matrix<Entry> mat(std::move(grid_letters));
        mat.reshape({rows, cols});
        return Grid(std::move(mat));
    } catch (const MatrixResizeError&) {
        // todo error handling
        throw;
    }

}
