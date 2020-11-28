#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <string>
#include "cell.h"

typedef std::vector<Cell> VC;
typedef std::vector<VC> VVC;

class Board
{
    private:
        std::string dfname;
    public:
        VVC grid;
        Board(int size);
        Board(std::string fname);
        bool load_from_file(std::string fname);
        int size();
        std::string get_data_filename();
        void print();
};

#endif // BOARD_H