/**
 * Grid declarations for project 3.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          Chandler Cree   <cree.d@northeastern.edu>
 * Date:    2020-10-25
 *
 */

#ifndef EECE_2560_PROJECTS_WORD_SEARCH_GRID_H
#define EECE_2560_PROJECTS_WORD_SEARCH_GRID_H


#include <string>
#include <vector>

class Grid {
  public:
    Grid() = default;

    void set_row_size(); //Prompts user to enter a size for row
    void set_col_size(); //Prompts user to enter a size for col

    int get_row_size(); //Returns the row size of the grid
    int get_col_size(); //Returns the col size of the grid

    static Grid read_chars(const char* file_name); //Reads the char values into the grid


  private:
    int row_size;
    int col_size;
};


#endif //EECE_2560_PROJECTS_WORD_SEARCH_GRID_H
