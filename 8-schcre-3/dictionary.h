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

#include <iosfwd>
#include <string>
#include <vector>

class Dictionary {

    std::vector<std::string> m_words;

  public:
    Dictionary() = default;

    explicit Dictionary(std::vector<std::string> words) : m_words(std::move(words)) {
        sort_words();
    }

    static Dictionary read_file(const char* file_name);

  private:
    void sort_words();
};

#endif //EECE_2560_PROJECTS_DICTIONARY_H
