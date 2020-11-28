#include "cell.h"

Cell::Cell()
{
    row = col = -1;
    val = 0;
}

Cell::Cell(int row, int col)
{
    this->row = row;
    this->col = col;
    this->val = 0;
}

Cell::Cell(int row, int col, int val)
{
    this->row = row;
    this->col = col;
    this->val = val;
}

bool Cell::is_empty()
{
    return this->val == 0;
}

bool Cell::operator==(const Cell& rhs)
{
    if (this == &rhs) return true;
    return this->row == rhs.row && this->col == rhs.col;
}