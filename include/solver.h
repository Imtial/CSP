#ifndef SOLVER_H
#define SOLVER_H

#include <list>
class Board;
class Cell;
typedef std::list<Cell*> LC;

class Solver
{
    public:
        Solver(Board &board);
        enum H {NONE, SDF, RANDOM, DOMDEG, DOMDDEG};
        enum ALGO {BT, FC, MAC};
    private:
        Board& board;
        int node_count;
        H heuristic;
        ALGO algo;
        LC empty_cells;

        bool is_valid(Cell &cell, int val);
        Cell& next_empty(Cell &cur);
        bool backtrack(Cell &cell);
        void assign(Cell &cell, int val, LC * old_doms = NULL);
        void bt_assign(Cell &cell, int val);
        void fc_assign(Cell &cell, int val, LC * old_doms);
        void resign(Cell &cell, LC * old_doms = NULL);
        void bt_resign(Cell &cell);
        void fc_resign(Cell &cell, LC * old_doms);
        void calc_dynamic_doms(Cell &cell);
    public:
        bool solve();
        void set_heuristic(const H heuristic);
        void set_algorithm(const ALGO algorithm);
        void report(const char * out_fname = NULL);
};

#define S_BT    "BACKTRACKING"
#define S_FC    "FORWARD CHECKING"
#define S_MAC   "MAINTAINING ARC CONSISTENCY"

#define S_NONE  "NO"
#define S_SDF   "SDF"
#define S_RAND  "RANDOM"
#define S_DOMDEG    "DOMDEG"
#define S_DOMDDEG   "DOMDDEG"

#endif // SOLVER_H