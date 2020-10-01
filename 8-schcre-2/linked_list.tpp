
#include "linked_list.h"

/**
 * EECE 2560 Project 2 - linked list definitions.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          David Cree      <cree.d@northeastern.edu>
 * Date:    2020-09-30
 *
 */

template<typename T>
typename LinkedList<T>::iterator  // typename keyword needed for dependent return type
LinkedList<T>::insert_after(LinkedList<T>::iterator position, const T& value)
{
    // Create a new node which takes ownership of the current "next node".
    auto new_node = BasicUnique<BaseNode>(new Node{
        {std::move(position.m_iter_pos->m_next_ptr)}, // Transfer node ownership
        value   // Copy value into new heap allocated node
    });

    // Set new_node as the current node's "next node".
    // The current node takes ownership of new_node.
    std::swap(position.m_iter_pos->m_next_ptr, new_node);

    // The local new_node no longer owns any memory.

    return position.next();
}

template<typename T>
void LinkedList<T>::push_front(const T& value)
{
    // Discard the return value.
    insert_after(before_begin(), value);
}

template<typename T>
void LinkedList<T>::remove_after(LinkedList::iterator position)
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
    // The former "next node" will be desctructed when tmp goes out of scope.
}

template<typename T>
void LinkedList<T>::clear()
{
    // Allow a specialized swap to be found through ADL
    // if we later define one [C.165 in 9 from header].
    using std::swap;

    // Locally scoped pointer for destroying the node that is removed.
    // Default initializes to nullptr.
    BasicUnique<BaseNode> tmp{};

    // Give ownership of the first element of the linked list to the temporary
    // variable. The list will be recursively destructed when the local variable
    // goes out of scope.
    swap(m_head.m_next_ptr, tmp);

}
