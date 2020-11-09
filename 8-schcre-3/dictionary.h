/**
 * Dictionary declarations for project 3.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          Chandler Cree   <cree.d@northeastern.edu>
 * Date:    2020-10-22
 *
 */

#ifndef EECE_2560_PROJECTS_DICTIONARY_H
#define EECE_2560_PROJECTS_DICTIONARY_H

#include <iostream>         // for I/O stream definitions
#include <string>           // for std::string
#include <vector>           // for std::vector

/**
 * A collection words.
 */
class Dictionary {

    /// The words in this dictionary.
    std::vector<std::string> m_words;

  public:
    /// The sorting algorithms that may be used to sort the dictionary.
    enum class SortingAlgorithm { SelectionSort, QuickSort, HeapSort };

    /// Creates a dictionary with no words.
    Dictionary() = default;

    /// Creates a dictionary with the given words.
    explicit Dictionary(std::vector<std::string> words, SortingAlgorithm algorithm = SortingAlgorithm::HeapSort)
        : m_words(std::move(words))
    {
        normalize_word();
        sort_words(algorithm);
    }

    /**
     * Creates a dictionary by reading the specified dictionary file.
     *
     * The file should contain one word per line.
     */
    static Dictionary read_file(const char* file_name, SortingAlgorithm algorithm = SortingAlgorithm::HeapSort);

    /// Returns true if the given word is contained in this dictionary.
    [[nodiscard]] bool contains(std::string_view key) const;

    friend std::ostream& operator<<(std::ostream& os, const Dictionary& dictionary);

  private:
    /// Sorts the words in this dictionary.
    void sort_words(SortingAlgorithm algorithm);

    /// Normalizes the words in this dictionary to lowercase strings.
    void normalize_word();
};

inline std::istream& operator>>(std::istream& in, Dictionary::SortingAlgorithm& algorithm)
{
    using underlying_type = std::underlying_type_t<Dictionary::SortingAlgorithm>;
    constexpr auto first = static_cast<underlying_type>(Dictionary::SortingAlgorithm::SelectionSort);
    constexpr auto last = static_cast<underlying_type>(Dictionary::SortingAlgorithm::HeapSort);

    underlying_type temp;
    in >> temp;

    // underling_type could be unsigned, so we can't test (temp < first) and
    // instead need to use inclusive bounds checks.
    if (!(temp >= first && temp <= last)) {
        in.setstate(std::ios::failbit);
    } else {
        algorithm = static_cast<Dictionary::SortingAlgorithm>(temp);
    }
    return in;
}

inline std::ostream& operator<<(std::ostream& out, Dictionary::SortingAlgorithm& algorithm)
{
    using SortingAlgorithm = Dictionary::SortingAlgorithm;

    switch (algorithm) {
        case SortingAlgorithm::SelectionSort: {
            out << "SelectionSort";
            break;
        }
        case SortingAlgorithm::QuickSort: {
            out << "QuickSort";
            break;
        }
        case SortingAlgorithm::HeapSort: {
            out << "HeapSort";
            break;
        }

    }
    return out;
}

#endif //EECE_2560_PROJECTS_DICTIONARY_H
