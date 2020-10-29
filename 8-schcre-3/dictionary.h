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

#include <iosfwd>           // for I/O stream declarations (iostream not necessary)
#include <string>           // for std::string
#include <vector>           // for std::vector

/**
 * A collection words.
 */
class Dictionary {

    /// The words in this dictionary.
    std::vector<std::string> m_words;

  public:
    /// Creates a dictionary with no words.
    Dictionary() = default;

    /// Creates a dictionary with the given words.
    explicit Dictionary(std::vector<std::string> words) : m_words(std::move(words)) {
        normalize_word();
        sort_words();
    }

    /**
     * Creates a dictionary by reading the specified dictionary file.
     *
     * The file should contain one word per line.
     */
    static Dictionary read_file(const char* file_name);

    /// Returns true if the given word is contained in this dictionary.
    [[nodiscard]] bool contains(std::string_view key) const;

    friend std::ostream& operator<<(std::ostream& os, const Dictionary& dictionary);

  private:
    /// Sorts the words in this dictionary.
    void sort_words();
    /// Normalizes the words in this dictionary to lowercase strings.
    void normalize_word();
};

#endif //EECE_2560_PROJECTS_DICTIONARY_H
