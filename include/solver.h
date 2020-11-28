#ifndef SOLVER_H
#define SOLVER_H

class Board;
class Cell;

class Solver
{
    public:
        Solver(Board &board);
        enum H {NONE, SDF};
    private:
        Board& board;
        int node_count;
        H heuristic;
        VVC empty_cells;
        bool is_valid(Cell &cell, int val);
        Cell& next_empty(Cell &cur, const H heuristic);
        bool backtrack(Cell &cell);
    public:
        bool backtrack();
        void report();
};

#endif // SOLVER_H