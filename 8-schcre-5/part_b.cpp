/**
 * Project 5b main file.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          Chandler Cree   <cree.d@northeastern.edu>
 * Date:    2020-12-07
 *
 */

#include <array>
#include <iostream>

#include "maze.h"

constexpr auto k_maze_files = std::array{
    "resources/maze1.txt",
    "resources/maze2.txt",
    "resources/maze3.txt"
};

int main()
{

    for (const auto file_name : k_maze_files) {
        std::cout << file_name << ":\n";
        const auto maze = Maze::read_file(file_name);
        auto graph = maze.make_graph();

        for (const auto& node : graph) {
            std::cout << node.index() << ":";
            for (const auto& neighbor : node.neighbors()) {
                std::cout << ' ' << neighbor.index();
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    }
}
