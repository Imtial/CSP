#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include "board.h"

std::vector<int8_t> dummy_domain;

Board::Board(int size)
{
    DOMAIN.clear();
    for (int i = 0; i < size; i++) DOMAIN.push_back(i+1);

    for (int i = 0; i < size; i++)
    {
        VC temp;
        for (int j = 0; j < size; j++)
        {
            temp.push_back(Cell(i, j));
            temp.back().doms = DOMAIN;
        }
        grid.push_back(temp);
    }
}

Board::Board(std::string fname)
{
    load_from_file(fname);
}

int Board::size()
{
    return grid.size();
}

bool Board::load_from_file(std::string fname)
{
    std::ifstream ifs(fname, std::ifstream::in);

    int size = -1;
    std::string line;

    // read N
    if (std::getline(ifs, line))
    {
        size = std::stoi(line.substr(line.find("=")+1, line.length()-1));
    }
    if (size < 0) return false;

    grid.resize(size, VC(size, Cell()));
    DOMAIN.clear();
    for (int i = 0; i < size; i++) DOMAIN.push_back(i+1);

    // just skip two lines
    for (int s = 0; s < 2; s++) std::getline(ifs, line);

    int i = -1;
    while (std::getline(ifs, line))
    {
        int pos = line.find("|");
        if (pos != -1)
        {
            line = line.substr(0, pos-1);
            i++;
        }
        std::stringstream ss(line);
        int j = 0;
        while (ss.good())
        {   
            std::string substr;
            std::getline(ss, substr, ',');
            grid[i][j].val = std::stoi(substr);
            grid[i][j].row = i;
            grid[i][j].col = j;
            grid[i][j].doms = DOMAIN;
            j++;
            // std::cout << std::stoi(substr) << "^";
        }
        // std::cout << std::endl;
    }
    ifs.close();
    return true;
}

void Board::print()
{
    for (VC &row : grid)
    {
        for (Cell &cell : row)
        {
            std::cout << std::setw(2) << int(cell.val) << " ";
        }
        std::cout << std::endl;
    }
}