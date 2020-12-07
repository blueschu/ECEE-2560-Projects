/**
 * Maze implementation for project 5.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          Chandler Cree   <cree.d@northeastern.edu>
 * Date:    2020-12-07
 *
 */

#include "maze.h"

Graph<Maze::Coordinate, Maze::PathWeight> Maze::make_graph() const
{
    const auto[max_row, max_col] = m_tiles.dimensions();
    std::vector<Coordinate> path_nodes;

    for (std::size_t row{0}; row < max_row; ++row) {
        for (std::size_t col{0}; col < max_col; ++col) {
            if (m_tiles[{row, col}] == Tile::Path) {
                path_nodes.emplace_back(row, col);
            }
        }
    }

    Graph<Coordinate, PathWeight> graph(std::move(path_nodes));

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
            // We ignore neighbors.
        }

    }

    return result;
}
