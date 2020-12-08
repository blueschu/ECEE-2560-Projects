/**
 * Maze implementation for project 5.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          Chandler Cree   <cree.d@northeastern.edu>
 * Date:    2020-12-07
 *
 */

#include "maze.h"

#include <algorithm>        // for std::find
#include <fstream>          // for file I/O stream definitions
#include <iterator>         // for std::istream_iterator
#include <sstream>          // for std::ostringstream

Maze Maze::read_file(const char* file_name)
{
    std::ifstream in_stream(file_name);
    in_stream.exceptions(std::ios::badbit);

    if (!in_stream) {
        throw std::runtime_error("maze file does not exist");
    }

    std::size_t rows;
    std::size_t cols;

    // Exception raised on invalid input since badbit is set.
    in_stream >> rows >> cols >> std::ws;

    std::vector<Tile> grid_letters;

    std::copy(
        std::istream_iterator<Tile>(in_stream),
        std::istream_iterator<Tile>(),
        std::back_inserter(grid_letters)
    );

    if (grid_letters.size() != rows * cols + 1) {
        throw std::runtime_error("invalid maze file format");
    }

    // Ignore trailing 'Z'.
    grid_letters.pop_back();

    Matrix<Tile> mat(std::move(grid_letters));
    mat.reshape({rows, cols});

    return Maze(std::move(mat));
}

Graph<Maze::Coordinate, Maze::PathWeight> Maze::make_graph() const
{
    const auto[max_row, max_col] = m_tiles.dimensions();
    std::vector<Coordinate> path_nodes;

    // Locate all passable tiles in the maze.
    for (std::size_t row{0}; row < max_row; ++row) {
        for (std::size_t col{0}; col < max_col; ++col) {
            if (m_tiles[{row, col}] == Tile::Path) {
                path_nodes.emplace_back(row, col);
            }
        }
    }

    // Create a graph (with no edges) with the passable tiles in the maze.
    Graph<Coordinate, PathWeight> graph(std::move(path_nodes));

    // Add an edge between each adjacent nodes in the maze.
    for (auto& node : graph) {
        for (const auto& neighbor : paths_from(*node)) {
            // std::find is guaranteed to succeed here since all tiles
            // containing a path will have an associated node in the graph.
            const auto connected_node = std::find_if(
                std::cbegin(graph),
                std::cend(graph),
                [&](const auto& n) { return *n == neighbor; }
            );
            node.connect(*connected_node, k_path_weight);
        }
    }

    return graph;

}

std::vector<Maze::Coordinate> Maze::paths_from(Maze::Coordinate pos) const
{
    std::vector<Coordinate> result;
    const Coordinate neighbors[]{
        {pos.first + 1, pos.second},        // North
        {pos.first, pos.second + 1},        // East
        {pos.first - 1, pos.second},        // South
        {pos.first, pos.second - 1},        // West
    };

    for (const auto& nb_coord : neighbors) {
        try {
            if (m_tiles[nb_coord] == Tile::Path) {
                result.push_back(nb_coord);
            }
        } catch (const MatrixIndexError&) {
            // We ignore invalid neighbors that fall off the edge of the maze.
        }
    }

    return result;
}

std::string Maze::directions_string(const std::vector<Coordinate>& path) const
{
    const auto[max_row, max_col] = m_tiles.dimensions();
    std::ostringstream stream;

    for (std::size_t row{0}; row < max_row; ++row) {
        for (std::size_t col{0}; col < max_col; ++col) {
            if (std::find(std::cbegin(path), std::cend(path), Coordinate{row, col}) != std::cend(path)) {
                stream << 'o';
            } else {
                stream << (m_tiles[{row, col}] == Maze::Tile::Path ? '.' : '#');
            }
        }
        stream << '\n';
    }

    return stream.str();
}

std::istream& operator>>(std::istream& in, Maze::Tile& tile)
{
    char symbol;
    in >> symbol;
    tile = (symbol == 'O') ? Maze::Tile::Path : Maze::Tile::Blocked;
    return in;
}
