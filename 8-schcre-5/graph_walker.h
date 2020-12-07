/**
 * Graph walker class for project 5.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          Chandler Cree   <cree.d@northeastern.edu>
 * Date:    2020-12-07
 *
 */

#ifndef EECE_2560_PROJECTS_GRAPH_WALKER_H
#define EECE_2560_PROJECTS_GRAPH_WALKER_H

#include <optional>         // for std::optional

#include "graph.h"

template<typename Node, typename Weight>
class GraphWalker {

    using GraphType = Graph<Node, Weight>;
    using NodeHandle = typename GraphType::NodeHandle;

    std::vector<bool> m_visited;

  public:

    struct PathSearchResult {
        std::vector<NodeHandle> path;
        Weight weight{};

        explicit operator bool() { return !path.empty(); }
    };

    GraphWalker() = default;

    PathSearchResult find_path_dfs(
        const GraphType& graph,
        const NodeHandle& start,
        const NodeHandle& goal)
    {
        prepare_for(graph);
        auto result = find_path_dfs_helper(start, goal);
        std::reverse(std::begin(result.path), std::end(result.path));
        return result;
    }

  private:

    void prepare_for(const GraphType& graph)
    {
        m_visited.resize(graph.size());
        std::fill(std::begin(m_visited), std::end(m_visited), 0);
    }

    PathSearchResult find_path_dfs_helper(
        const NodeHandle& current,
        const NodeHandle& goal
    )
    {
        m_visited[current.index()] = true;

        if (current.index() == goal.index()) {
            return {{goal}, Weight{}};
        }

        for (const auto&[neighbor, weight] : current.neighbors()) {
            if (m_visited[neighbor.index()]) {
                continue;
            }

            auto result = find_path_dfs_helper(neighbor, goal);

            if (result) {
                result.path.push_back(current);
                result.weight += weight;
                return result;
            }

        }
        m_visited[current.index()] = false;
        return {{}, {}};
    }

};

#endif //EECE_2560_PROJECTS_GRAPH_WALKER_H
