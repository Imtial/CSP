#include "solver.h"
#include "board.h"
#include "cell.h"
#include <iostream>

Cell dummy_cell;

Solver::Solver(Board &board) : board(board) {}

bool Solver::backtrack()
{
    node_count = 0;
    return backtrack(board.grid[0][0]);
}

bool Solver::backtrack(Cell &cell)
{
    Cell& emp_cell = next_empty(cell, Solver::NONE);
    node_count++;

    // no more empty cell
    if (emp_cell == INVALID_CELL) return true;


    for (int8_t& val : emp_cell.doms)
    {
        if (is_valid(emp_cell, val))
        {
            emp_cell.val = val;

            if (backtrack(emp_cell)) return true;

            emp_cell.val = 0;
        }
    }

    return false;
}

Cell& Solver::next_empty(Cell &cur, const H heuristics)
{
    if (heuristics == Solver::NONE)
    {
        // search in current row
        for (int c = cur.col; c < board.size(); c++)
            if (board.grid[cur.row][c].is_empty()) return board.grid[cur.row][c];
        
        // search from next row
        for (int r = cur.row+1; r < board.size(); r++)
        {
            for (int c = 0; c < board.size(); c++)
                if (board.grid[r][c].is_empty()) return board.grid[r][c];
        }
    }
    else if (heuristics == Solver::SDF)
    {
        std::cout << "SDF" << std::endl;
    }
    return INVALID_CELL;
}

bool Solver::is_valid(Cell &cell, int val)
{
    // check if any row in the same column has the same value
    for (int r = 0; r < board.size(); r++)
    {
        if (board.grid[r][cell.col].val == val) return false;
    }

    // check if any column in the same row has the same value
    for (int c = 0; c < board.size(); c++)
    {
        if (board.grid[cell.row][c].val == val) return false;
    }

    // both check passed. so no conflict
    return true;
}

void Solver::report()
{
    std::cout << "Node Count = " << node_count << std:: endl;
}