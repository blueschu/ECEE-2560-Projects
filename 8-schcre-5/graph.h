/**
 * Graph class for project 5.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          Chandler Cree   <cree.d@northeastern.edu>
 * Date:    2020-12-07
 *
 * References
 * ===========
 *  [1] https://en.cppreference.com/w/cpp/named_req/Container
 *  [2] https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
 *  [3] https://stackoverflow.com/questions/856542/
 */

#ifndef EECE_2560_PROJECTS_GRAPH_H
#define EECE_2560_PROJECTS_GRAPH_H

#include <algorithm>        // for std::transform
#include <optional>         // for std::optional

#include "matrix.h"

/**
 * A directed graph that stores edges using an adjacency matrix.
 *
 * @tparam Node Type stored in the nodes of the graph.
 * @tparam Weight Type used to represent edge weights in the graph.
 */
template<typename Node, typename Weight>
class Graph {
  public:
    class NodeHandle;

    // Type aliases for C++ container [1].
    using value_type = NodeHandle;
    using reference = NodeHandle&;
    using const_reference = const NodeHandle&;
    using iterator = typename std::vector<value_type>::iterator;
    using const_iterator = typename std::vector<value_type>::const_iterator;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;

    /**
     * Wrapper around node values to provide graph-aware functionality, like
     * accessing neighbors and adding new edges.
     */
    class NodeHandle {
        /// The graph this node is associated with.
        Graph* m_graph;

        /// This node's index in its graph.
        size_type m_index;

        /// This node's contents.
        Node m_node;

        /// Creates a handle for the given node in the given graph, which is
        /// is located at the specified index in the graph.
        NodeHandle(Graph& graph, size_type index, Node node)
            : m_graph(&graph), m_index(index), m_node(std::move(node)) {}

      public:
        /// This node's index in its graph.
        [[nodiscard]] size_type index() const noexcept { return m_index; }

        /**
         * Creates an edge from this node to the given node with the specified
         * edge weight.
         *
         * @param other New neighbor for this node.
         * @param weight Edge weight for the new edge.
         */
        void connect(const NodeHandle& other, const Weight& weight)
        {
            m_graph->connect_indices(m_index, other.m_index, weight);
        }

        /**
         * Creates an edge from this node to the given node with the specified
         * edge weight.
         *
         * @param other New neighbor for this node.
         * @param weight Edge weight for the new edge.
         */
        void connect(const NodeHandle& other, Weight&& weight)
        {
            m_graph->connect_indices(m_index, other.m_index, std::forward<Weight>(weight));
        }

        /**
         * Returns a the neighbors of this node. Each neighbor is described by
         * a pair containing 1) a handle for the neighboring node, and 2) a
         * reference to the weight value of the edge that connects the two nodes.
         *
         * @return The neighbors of this node.
         */
        std::vector<std::pair<NodeHandle, const Weight&>> neighbors() const
        {
            const auto row = m_index;
            std::vector<std::pair<NodeHandle, const Weight&>> result;

            for (size_type col{0}; col < m_graph->m_edges.dimensions().second; ++col) {
                if (const std::optional<Weight>& weight = m_graph->m_edges[{row, col}]) {
                    result.emplace_back(m_graph->m_nodes[col], *weight);
                }
            }
            return result;
        }

        Node& operator*() noexcept { return m_node; }

        const Node& operator*() const noexcept { return m_node; }

        Node* operator->() noexcept { return &m_node; }

        const Node* operator->() const noexcept { return &m_node; }

        friend Graph;
    };

  private:
    /// The nodes contained in this graph.
    std::vector<value_type> m_nodes;

    /// Adjacency matrix representing the edges of this graph.
    Matrix<std::optional<Weight>> m_edges;

  public:
    /// Creates a graph with the given nodes.
    explicit Graph(std::vector<Node> nodes)
        : m_edges(std::vector(nodes.size() * nodes.size(), std::optional<Weight>{}))
    {
        size_type counter{0};
        std::transform(
            std::make_move_iterator(std::begin(nodes)),
            std::make_move_iterator(std::end(nodes)),
            std::back_inserter(m_nodes),
            [&](Node&& n) { return NodeHandle(*this, counter++, std::move(n)); }
        );
        m_edges.reshape({m_nodes.size(), m_nodes.size()});
    }

    /**
     * Creates an edge between the nodes specified by the given node indices.
     *
     * If an edge already existing between the nodes, if will be overwritten.
     *
     * @param from Start node for the new edge.
     * @param to End node for the new edge.
     * @param weight Edge weight for the new edge.
     */
    void connect_indices(size_type from, size_type to, const Weight& weight)
    {
        m_edges[{from, to}] = weight;
    }

    /**
     * Creates an edge between the nodes specified by the given node indices.
     *
     * If an edge already existing between the nodes, if will be overwritten.
     *
     * @param from Start node for the new edge.
     * @param to End node for the new edge.
     * @param weight Edge weight for the new edge.
     */
    void connect_indices(size_type from, size_type to, Weight&& weight)
    {
        m_edges[{from, to}] = std::make_optional(std::forward<Weight>(weight));
    }

    reference operator[](size_type index)
    {
        // Safely delegate to const implementation [3].
        return const_cast<reference>(
            static_cast<const Graph*>(this)->operator[](index)
        );
    }

    const_reference operator[](size_type index) const
    {
        return m_nodes[index];
    }

    /// Returns the number of nodes in this graph.
    [[nodiscard]] std::size_t size() const { return m_nodes.size(); }

    iterator begin() noexcept { return std::begin(m_nodes); }

    iterator end() noexcept { return std::end(m_nodes); }

    const_iterator begin() const noexcept { return std::begin(m_nodes); }

    const_iterator end() const noexcept { return std::end(m_nodes); }
};

#endif //EECE_2560_PROJECTS_GRAPH_H
