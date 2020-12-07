/**
 * Maze class for project 5.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          Chandler Cree   <cree.d@northeastern.edu>
 * Date:    2020-12-07
 *
 */

#ifndef EECE_2560_PROJECTS_MAZE_H
#define EECE_2560_PROJECTS_MAZE_H

#include <algorithm>        // for std::find

#include "matrix.h"
#include "graph.h"

class Maze {

  public:
    // Avoid vector<bool>
    enum class Tile { Path, Blocked };

    using Coordinate = Matrix<Tile>::Coordinate;

    using PathWeight = int;

    constexpr static PathWeight k_path_weight{1};

    Matrix<Tile> m_tiles;

    [[nodiscard]] Graph<Coordinate, PathWeight> make_graph() const;

    [[nodiscard]] std::vector<Coordinate> paths_from(Coordinate pos) const;
};

#endif //EECE_2560_PROJECTS_MAZE_H
