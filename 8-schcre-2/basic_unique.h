/**
 * EECE 2560 Project 2 - Card class declarations.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          David Cree      <cree.d@northeastern.edu>
 * Date:    2020-09-30
 *
 * References
 * ==========
 *
 *  [1] https://en.cppreference.com/w/cpp/utility/exchange
 *  [2] https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
 */

#ifndef EECE_2560_PROJECTS_BASIC_UNIQUE_H
#define EECE_2560_PROJECTS_BASIC_UNIQUE_H

#include <utility>      // for std::exchange

/**
 * A smart pointer that uniquely owns its contents.
 *
 * This class is an approximate substitute for std::unique_ptr. For this
 * project, we were required to perform manual memory allocations
 * using `new` and `delete` to store nodes in a linked list. In order to
 * save some headaches over reasoning about pointer ownership, we decided
 * to implement our own mock smart pointer with roughly the same semantics
 * as those include in the C++ standard library. Throughout the linked list
 * code, each instance of BasicUnique should be able to be replaced by
 * std::unique_ptr with no obvious change in behavior.
 *
 * This should make our linked list implementation much easier to read
 * since only basic knowledge of move semantics is required to understand
 * how memory is being managed.
 *
 * @tparam T The value type referenced by this pointer.
 */
template<typename T>
class BasicUnique {
    /// The owning pointer to the resource managed by this class.
    T* m_ptr{nullptr};

  public:

    /*
     * Default constructor.
     *
     * All members are already given in-class member initializers, so we can
     * just use the compiler generated default constructor [C.45,C.80 in 2].
     */
    BasicUnique() = default;

    /*
     * Construct a BasicUnique that will the resources references by the given
     * pointer.
     */
    explicit BasicUnique(T* owned_ptr) : m_ptr{owned_ptr} {}

    // Disallow copy constructor [C.81 in 2].
    BasicUnique(const BasicUnique&) noexcept = delete;

    // Move constructor
    BasicUnique(BasicUnique&& other) noexcept
        : m_ptr{std::exchange(other.m_ptr, nullptr)} {}

    // Destructor.
    ~BasicUnique()
    {
        delete m_ptr;
    }

    // Disallow copy assignment [C.81 in 2].
    BasicUnique& operator=(const BasicUnique&) noexcept = delete;

    // Move assignment
    BasicUnique& operator=(BasicUnique&& other) noexcept
    {
        // Use std::swap explicitly since don't care about specialization here.
        std::swap(m_ptr, other.m_ptr);
        return *this;
    }

    // Dereference operator overload.
    T& operator*() const noexcept { return *m_ptr; }

    // Arrow operator overload.
    T* operator->() const noexcept { return m_ptr; }

    /**
     * Returns the owned pointer without releasing ownership.
     *
     * @returns Non-owning pointer to stored resource.
     */
    T* get() const noexcept
    {
        return m_ptr;
    }

};

#endif //EECE_2560_PROJECTS_BASIC_UNIQUE_H
