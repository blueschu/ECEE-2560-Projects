/**
 * EECE 2560 Project 2 - linked list definitions.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          Chandler Cree   <cree.d@northeastern.edu>
 * Date:    2020-09-30
 *
 */

template<typename T>
auto LinkedList<T>::insert_after(iterator position, const T& value) -> iterator
{
    // Allow a specialized swap to be found through ADL
    // if we later define one [C.165 in 9 from header].
    using std::swap;

    // Create a new node which takes ownership of the current "next node".
    auto new_node = BasicUnique<BaseNode>(new Node{
        // Transfer ownership of "next node". The current node now points to nullptr.
        {std::move(position.m_iter_pos->m_next_ptr)},
        value   // Copy value into new heap allocated node
    });

    // Set new_node as the current node's "next node".
    // The current node takes ownership of new_node.
    swap(position.m_iter_pos->m_next_ptr, new_node);

    // The local new_node no longer owns any memory.

    return position.next();
}

template<typename T>
void LinkedList<T>::clear()
{
    // Given ownership of this linked list to a local variable. This list
    // will now be in its "moved from" state, which is a valid empty list. The
    // old list will be destroyed by the destructor when this function exits.
    LinkedList<T> tmp = std::move(*this);
}

template<typename T>
void LinkedList<T>::remove_after(iterator position)
{
    // Allow a specialized swap to be found through ADL
    // if we later define one [C.165 in 9 from header].
    using std::swap;

    // Reference to the pointer held by the current node for convenience.
    BasicUnique<BaseNode>& next_node_ptr = position.m_iter_pos->m_next_ptr;

    // Locally scoped pointer for destroying the node that is removed.
    // Default initializes to nullptr.
    BasicUnique<BaseNode> tmp{};

    // Given ownership of the node after "next node" to the temporary variable
    // on the stack.
    // This is the node that we want the current node to own.
    swap(tmp, next_node_ptr->m_next_ptr);
    // Swap the node after "next node" into the current node, and give the
    // pointer to the "next node" to the temporary.
    swap(tmp, next_node_ptr);
    // The former "next node" will be destructed when tmp goes out of scope.
}
