#ifndef CELL_H
#define CELL_H

#include <cstdint>
#include <vector>

class Cell
{
    public:
        int8_t row, col, val, deg;
        std::vector<int8_t> doms;
        Cell();
        Cell(int row, int col);
        Cell(int row, int col, int val);
        bool is_empty();
        bool operator==(const Cell& rhs);
};

extern Cell dummy_cell;
#define INVALID_CELL    dummy_cell

extern std::vector<int8_t> dummy_domain;
#define DOMAIN  dummy_domain

#endif // CELL_H