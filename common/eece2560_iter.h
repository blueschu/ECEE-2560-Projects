/**
 * Common iterator utilities used in project 2 and beyond.
 *
 * For ease of user, these utilities are implemented as a header-only library.
 *
 * References
 * ==========
 *  [1] https://en.cppreference.com/w/cpp/ranges/filter_view
 *  [2] https://en.cppreference.com/w/cpp/iterator/iterator_traits
 *  [3] https://en.cppreference.com/w/cpp/named_req/ForwardIterator
 */


#ifndef EECE_2560_PROJECTS_EECE2560_ITER_H
#define EECE_2560_PROJECTS_EECE2560_ITER_H

#include <iterator>         // for std::iterator_traits

namespace eece2560 {

/**
 * An iterator that filters elements satisfying a predicate from a given
 * iterator range.
 *
 * This iterator can be used as a forward iterator if the underlying iterators
 * meet the requirements of a forward iterator. Otherwise, this iterator is
 * only an input iterator.
 *
 * @tparam Iter Iterator type being wrapped.
 * @tparam Pred Callable indicating whether an element from the iterator range
 *              should be yielded.
 */
template<typename Iter, typename Pred>
class FilterIter {
  public:
    // Iterator traits [2].
    using value_type = typename std::iterator_traits<Iter>::value_type;
    using pointer = typename std::iterator_traits<Iter>::pointer;
    using reference = typename std::iterator_traits<Iter>::reference;
    using difference_type = typename std::iterator_traits<Iter>::difference_type;
    // If Iter meets the requirements of a forward iterator, then so does this
    // iterator. Otherwise, this iterator is only an input iterator [3].
    // For simplicity we do not add support for bi-directional iteration.
    using iterator_category = std::common_type_t<
        typename std::iterator_traits<Iter>::iterator_category,
        std::forward_iterator_tag
    >;

  private:
    /**
     * The current iterator.
     */
    Iter m_iter;

    /**
     * End iterator.
     */
    Iter m_end{};

    /**
     * Predicate indicating whether or not an element should be yielded.
     */
    Pred m_predicate;

  public:
    /**
     * Creates a FilterIter over the given range with the given filter predicate.
     *
     * @param it,end Iterator range.
     * @param pred Filter predicate.
     */
    constexpr FilterIter(Iter it, Iter end, Pred pred)
        : m_iter{it}, m_end{end}, m_predicate{pred}
    {
        advance();
    }

    /*
     * Dereference operator overload.
     *
     */
    reference operator*() const { return m_iter.operator*(); }

    pointer operator->() const { return m_iter.operator->(); }

    /*
     * Comparison operators.
     */
    bool operator==(FilterIter other) const { return m_iter == other.m_iter; }

    bool operator!=(FilterIter other) const { return !(*this == other); }

    // Pre-increment overload.
    FilterIter& operator++()
    {
        if (m_iter != m_end) {
            ++m_iter;
        }
        advance();
        return *this;
    }

    // Post-increment overload.
    FilterIter operator++(int)
    {
        auto temp = *this;
        ++(*this);
        return temp;
    }

  private:
    /**
     * Repeatedly increments this iterator's underlying iterator until either
     * the predicate is satisfied or the end is reached.
     */
    void advance()
    {
        while (m_iter != m_end && !m_predicate(*m_iter)) {
            ++m_iter;
        }
    }

};

template<typename T, typename Pred>
constexpr auto make_filter_iter(const T& collection, Pred&& pred)
{
    return FilterIter{std::begin(collection), std::end(collection), std::forward<Pred>(pred)};
}

template<typename T, typename Pred>
constexpr auto make_filter_iter_end(const T& collection, Pred&& pred)
{
    return FilterIter{std::end(collection), std::end(collection), std::forward<Pred>(pred)};
}

} // end namespace eece2560

#endif //EECE_2560_PROJECTS_EECE2560_ITER_H
