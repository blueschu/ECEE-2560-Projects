/**
 * Project 4a main file.
 *
 * Authors: Brian Schubert  <schubert.b@northeastern.edu>
 *          Chandler Cree   <cree.d@northeastern.edu>
 * Date:    2020-11-14
 *
 */

#include <iostream>

#include "suduko_board.h"

int main() {

    SudukoBoard<3> board;
    board.set_cell({0, 1}, 2);
    std::cout << board << '\n';

//    std::stringstream stream(".....2.......7...17..3...9.8..7......2.89.6...13..6....9..5.824.....891..........");
//    std::stringstream stream("01234567890abAB..17..3...9.8..7......2.89.6...13..6....9..5.824.....891.......");       // too short
    std::stringstream stream("01234567890abAB..17..3...9.8..7......2.89.6...13..6....9..5.824.....891.......321111111111111111");       // too long

    stream >> board;
    std::cout << board;
}
