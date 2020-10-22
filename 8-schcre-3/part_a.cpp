/**
 * Project 3a main file.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          Chandler Cree   <cree.d@northeastern.edu>
 * Date:    2020-10-22
 *
 */

#include "dictionary.h"

constexpr const char* DICTIONARY_FILE = "resources/dictionary.txt";

int main()
{

    auto dictionary = Dictionary::read_file(DICTIONARY_FILE);
}
