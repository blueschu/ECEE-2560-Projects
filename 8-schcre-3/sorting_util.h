/**
 * Sorting utilities for project 3.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          Chandler Cree   <cree.d@northeastern.edu>
 * Date:    2020-10-22
 *
 * References
 * ==========
 *  [1] https://en.cppreference.com/w/cpp/algorithm/iter_swap
 */

#ifndef EECE_2560_PROJECTS_SORTING_UTIL_H
#define EECE_2560_PROJECTS_SORTING_UTIL_H

#include <algorithm>        // std::iter_swap
#include <functional>       // for std::less
#include <utility>          // for std::swap

/**
 * Returns the minimum contained in the range [it, end).
 *
 * This function is analogous to the function std::min_element from standard
 * library.
 *
 * @tparam Iter Forward iterator type.
 * @tparam Comp Callable type to compare range elements.
 * @param it,end Range to be examined.
 * @param compare Function object used to compare elements.
 * @return Minimum element in the range.
 */
template<typename Iter, typename Comp = std::less<>>
Iter min_elem(Iter it, Iter end, Comp compare = Comp())
{
    auto curr_min = it;
    ++it;
    while (it != end) {
        if (compare(*it, *curr_min)) {
            curr_min = it;
        }
    }
    return curr_min;
}

/**
 * Sorts the given range using a selection sort implementation. This sorting
 * algorithm is stable.
 *
 * Runs in O(n^2) time and O(1) space.
 *
 * @tparam Iter Forward iterator type.
 * @param it,end Range to be sorted
 */
template<typename Iter>
void selection_sort(Iter it, Iter end)
{
    while (it != end) {
        std::iter_swap(it, min_elem(it, end));
        ++it;
    }
}

#endif //EECE_2560_PROJECTS_SORTING_UTIL_H
