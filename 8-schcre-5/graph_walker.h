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

template<typename Node, typename Edge>
class GraphWalker {

    using GraphType = Graph<Node, Edge>;
    using NodeHandle = typename GraphType::NodeHandle;

    std::vector<bool> m_visited;

  public:
    GraphWalker() = default;

    std::vector<NodeHandle> find_path_dfs(
        const GraphType& graph,
        const NodeHandle& start,
        const NodeHandle& goal)
    {
        prepare_for(graph);
        auto path = find_path_dfs_helper(start, goal);
        std::reverse(std::begin(path), std::end(path));
        return path;
    }

  private:

    void prepare_for(const GraphType& graph)
    {
        m_visited.resize(graph.size());
        std::fill(std::begin(m_visited), std::end(m_visited), 0);
    }

    std::vector<NodeHandle> find_path_dfs_helper(
        const NodeHandle& current,
        const NodeHandle& goal
    )
    {
        m_visited[current.index()] = true;

        if (current.index() == goal.index()) {
            return {goal};
        }

        for (const auto& [neighbor, _edge] : current.neighbors()) {
            if (m_visited[neighbor.index()]) {
                continue;
            }

            auto maybe_path = find_path_dfs_helper(neighbor, goal);

            if (!maybe_path.empty()) {
                maybe_path.push_back(current);
                return maybe_path;
            }

        }
        m_visited[current.index()] = false;
        return {};
    }

};

#endif //EECE_2560_PROJECTS_GRAPH_WALKER_H
