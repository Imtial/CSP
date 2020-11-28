#include "solver.h"
#include "board.h"
#include "cell.h"
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>

Cell dummy_cell;

Solver::Solver(Board &board) : board(board), heuristic(Solver::NONE), algo(Solver::BT)
{
    for (VC& row : board.grid)
    {
        for (Cell& cell : row)
        {
            if (cell.val == 0) empty_cells.push_back(&cell);
        }
    }
}

bool Solver::solve()
{
    node_count = 0;
    return backtrack(board.grid[0][0]);
}

bool Solver::backtrack(Cell &cell)
{
    Cell& emp_cell = next_empty(cell);
    node_count++;
    if (!(node_count % 10000000))
    std::cout << node_count << " " << empty_cells.size() << std::endl;

    // no more empty cell
    if (emp_cell == INVALID_CELL) return true;


    for (int8_t& val : emp_cell.doms)
    {
        if (is_valid(emp_cell, val))
        {
            // old domain set needed for reset in case of fc
            LC old_doms;
            assign(emp_cell, val, &old_doms);

            if (backtrack(emp_cell)) return true;

            resign(emp_cell, &old_doms);
        }
    }

    return false;
}

Cell& Solver::next_empty(Cell &cur)
{
    if (!empty_cells.empty())
    {
        if (heuristic == Solver::NONE)
        {
            return *empty_cells.front();
        }
        else if (heuristic == Solver::SDF)
        {
            long unsigned int min = board.size() + 1;
            LC::iterator it_remove;
            for (LC::iterator it = empty_cells.begin(); it != empty_cells.end(); ++it)
            {
                Cell * cell = *it;
                if (cell->doms.size() < min)
                {
                    min = cell->doms.size();
                    it_remove = it;
                }
                // std::cout << "(" << int((*it)->row) << "," << int((*it)->col) << ") ";
            }
            // std::cout << std::endl << "TO REMOVE: "<< "(" << int((*it_remove)->row) << "," << int((*it_remove)->col) << ") " << std::endl;
            std::swap(*empty_cells.begin(), *it_remove);
            // for (LC::iterator it = empty_cells.begin(); it != empty_cells.end(); ++it)
            // {
            //     std::cout << "(" << int((*it)->row) << "," << int((*it)->col) << ") ";
            // }
            // std::cout << std::endl;
            return *empty_cells.front();
        }   
        else if (heuristic == Solver::RANDOM)
        {
            // list is randomized while setting heuristic
            
            return *empty_cells.front();
        }
        else if (heuristic == Solver::DOMDEG)
        {
            float ratio = 0.0;
            float min = 1000.0;
            LC::iterator it_remove = empty_cells.begin();
            for (LC::iterator it = empty_cells.begin(); it != empty_cells.end(); ++it)
            {
                ratio = float((*it)->doms.size() / (*it)->deg);
                if (ratio < min)
                {
                    min = ratio;
                    it_remove = it;
                }
            }
            std::swap(*empty_cells.begin(), *it_remove);
            return *empty_cells.front();
        }
        else if (heuristic == Solver::DOMDDEG)
        {
            float ratio = 0.0;
            float min = 1000.0;
            LC::iterator it_remove = empty_cells.begin();
            for (LC::iterator it = empty_cells.begin(); it != empty_cells.end(); ++it)
            {
                // std::cout << int((*it)->deg) << " ";
                calc_dynamic_doms(*(*it));
                // std::cout << int((*it)->deg) << std::endl;
                ratio = ((*it)->deg != 0) ? float((*it)->doms.size() / (*it)->deg) : 0;
                if (ratio < min)
                {
                    min = ratio;
                    it_remove = it;
                }
            }
            std::swap(*empty_cells.begin(), *it_remove);
            return *empty_cells.front();
        }
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

void Solver::assign(Cell &cell, int val, LC * old_doms)
{
    empty_cells.pop_front();
    switch (algo)
    {
        case Solver::BT:
            bt_assign(cell, val);
            break;
        case Solver::FC:
            fc_assign(cell, val, old_doms);
            break;
        case Solver::MAC:
            break;
        
        default:
            std::cout << "ERROR in assign(): undefined algorithm" << std::endl;
            break;
    }
}

void Solver::bt_assign(Cell &cell, int val)
{
    cell.val = val;
}

void Solver::fc_assign(Cell &cell, int val, LC * old_doms)
{
    cell.val = val;
    for (int i = 0; i < board.size(); i++)
    {
        long unsigned int n;
        if (board.grid[cell.row][i].is_empty())
        {
            n = board.grid[cell.row][i].doms.size();
            board.grid[cell.row][i].doms
                .erase(
                    std::remove(board.grid[cell.row][i].doms.begin(), 
                        board.grid[cell.row][i].doms.end(), val), 
                    board.grid[cell.row][i].doms.end()
            );
            if (board.grid[cell.row][i].doms.size() < n) old_doms->push_back(&board.grid[cell.row][i]);
        }
        if (board.grid[i][cell.col].is_empty())
        {
            n = board.grid[i][cell.col].doms.size();
            board.grid[i][cell.col].doms
                .erase(
                    std::remove(board.grid[i][cell.col].doms.begin(), 
                        board.grid[i][cell.col].doms.end(), val), 
                    board.grid[i][cell.col].doms.end()
            );
            if (board.grid[i][cell.col].doms.size() < n) old_doms->push_back(&board.grid[i][cell.col]);
        }
    }
}

void Solver::resign(Cell &cell, LC * old_doms)
{
    empty_cells.push_front(&cell);
    switch (algo)
    {
        case Solver::BT:
            bt_resign(cell);
            break;
        case Solver::FC:
            fc_resign(cell, old_doms);
            break;
        case Solver::MAC:
            break;
        
        default:
            std::cout << "ERROR in resign(): undefined algorithm" << std::endl;
            break;
    }
}

void Solver::bt_resign(Cell &cell)
{
    cell.val = 0;
}

void Solver::fc_resign(Cell &cell, LC * old_doms)
{
    for (Cell*& c : *old_doms)
    {
        c->doms.push_back(cell.val);
    }
    cell.val = 0;
}

void Solver::set_algorithm(const ALGO algorithm)
{
    this->algo = algorithm;
    if (algo == Solver::FC)
    {
        for (Cell*& cell : empty_cells)
        {
            // tracks unassigned values
            std::vector<bool> tracker(board.size()+1, false);
            // set true to the value already assigned
            for (int i = 0; i < board.size(); i++)
            {
                tracker[board.grid[cell->row][i].val] = true;
                tracker[board.grid[i][cell->col].val] = true;
            }
            // clear cell's domain
            board.grid[cell->row][cell->col].doms.clear();
            // push unassigned values to tha domain
            for (int i = 1; i <= board.size(); i++)
            {
                if (!tracker[i]) board.grid[cell->row][cell->col].doms.push_back(i);
            }
            // std::cout << "("<<r<<","<<c<<"): " << board.grid[r][c].doms.size() << std::endl;
        }
    }
}

void Solver::set_heuristic(const H heuristic)
{
    this->heuristic = heuristic;
    if (heuristic == Solver::RANDOM)
    {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist(0, empty_cells.size()-2);
        for (int i = 0; i < int(empty_cells.size()/10); i++)
        {
            int pos = dist(rng);
            LC::iterator it_remove = empty_cells.begin();
            std::advance(it_remove, pos);
            std::swap(*empty_cells.begin(), *it_remove);
            // std::cout << empty_cells.size() << std::endl;
        }

    }
    else if (heuristic == Solver::DOMDEG)
    {
        for (Cell*& cell : empty_cells)
        {
            calc_dynamic_doms(*cell);
        }
    }
}

void Solver::calc_dynamic_doms(Cell &cell)
{
    int deg = 0;
    for (int i = 0; i < board.size(); i++)
    {
        if (board.grid[cell.row][i].is_empty()) deg++;
        if (board.grid[i][cell.col].is_empty()) deg++;
    }
    if (cell.is_empty()) deg -= 2;
    cell.deg = deg;
}

void Solver::report(const char * out_fname)
{
    std::ostream * out = &std::cout;

    if (out_fname != NULL) 
    {
        out = new std::ofstream(out_fname, std::ofstream::app);
    }
    std::string strAlgo;
    std::string strHeu;
    switch (algo)
    {
        case Solver::BT:
            strAlgo = std::string(S_BT);
            break;
        
        case Solver::FC:
            strAlgo = std::string(S_FC);
            break;
        
        case Solver::MAC:
            strAlgo = std::string(S_MAC);
            break;
    }
    switch (heuristic)
    {
        case Solver::NONE:
            strHeu = std::string(S_NONE);
            break;
        
        case Solver::SDF:
            strHeu = std::string(S_SDF);
            break;
        
        case Solver::RANDOM:
            strHeu = std::string(S_RAND);
            break;

        case Solver::DOMDEG:
            strHeu = std::string(S_DOMDEG);
            break;

        case Solver::DOMDDEG:
            strHeu = std::string(S_DOMDDEG);
            break;
    }
    *out << "DATA FILE: " << board.get_data_filename() << std::endl;
    *out << "           N = " << board.size() << std::endl;
    *out << "             Approach: " << strAlgo << " with " << strHeu << " value ordering" << std::endl;
    *out << "             Node Count = " << node_count << std:: endl << std::endl;

    if (out_fname != NULL) delete out;
}