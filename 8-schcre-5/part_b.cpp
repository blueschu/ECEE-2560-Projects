/**
 * Project 5b main file.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          Chandler Cree   <cree.d@northeastern.edu>
 * Date:    2020-12-07
 *
 */

#include <iostream>
#include "graph.h"
#include "maze.h"

int main()
{
    Graph<int, int> g(std::vector{1, 2});

    auto it = std::begin(g);

    auto first = *it;

    ++it;

    first.connect(*it, 1);

    for (auto n : first.neighbors()) {
        std::cout << n.index() << '\n';
    }

    auto maze = Maze::read_file("resources/maze1.txt");
    auto graph = maze.make_graph();

    for (const auto& node : graph) {
        std::cout << node.index() << ":";
        for (const auto& neighbor : node.neighbors()) {
            std::cout << ' ' << neighbor.index();
        }
        std::cout << '\n';
    }
}
