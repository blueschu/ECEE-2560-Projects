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

class Dictionary {

    std::vector<std::string> m_words;

  public:
    Dictionary() = default;

    explicit Dictionary(std::vector<std::string> words) : m_words(std::move(words)) {
        normalize_word();
        sort_words();
    }

    static Dictionary read_file(const char* file_name);

    [[nodiscard]] bool contains(std::string_view key) const;

    friend std::ostream& operator<<(std::ostream& os, const Dictionary& dictionary);

  private:
    void sort_words();
    void normalize_word();
};

#endif //EECE_2560_PROJECTS_DICTIONARY_H
