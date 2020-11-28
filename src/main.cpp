#include <iostream>
#include "../include/board.h"
#include "../include/solver.h"
using namespace std;

bool test(Board &board)
{
    int count = 0;
    for (int i = 0; i <= board.size(); i++)
    {
        for (int c = 0; c < board.size(); c++)
        {
            count = 0;
            for (int r = 0; r < board.size(); r++)
            {
                if (board.grid[r][c].val == i)
                {
                    if (++count > 1) return false;
                }
            }
        }

        for (int r = 0; r < board.size(); r++)
        {
            count = 0;
            for (int c = 0; c < board.size(); c++)
            {
                if (board.grid[r][c].val == i)
                {
                    if (++count > 1) return false;
                }
            }
        }
    }
    return true;
}

int main()
{
    cout << "Constraint Staisfaction Problem - Latin Square" << endl;

    string data_files [] = {
        "csp_task/data/d-10-01.txt.txt",
        "csp_task/data/d-10-06.txt.txt",
        "csp_task/data/d-10-07.txt.txt",
        "csp_task/data/d-10-08.txt.txt",
        "csp_task/data/d-10-09.txt.txt",
        "csp_task/data/d-15-01.txt.txt"
    };

    string out_file = "report.doc";

    for (string &dfile : data_files)
    {
        Board board(dfile);
        Board board_cpy = board;

        Solver solver_bt(board_cpy);
        bool f = solver_bt.solve();

        board_cpy.print();
        solver_bt.report(out_file.c_str());
        if (!test(board_cpy)) cout << "WRONG" << endl;

        for (int h = Solver::NONE; h != Solver::DOMDDEG; h++)
        {
            board_cpy = board;
            Solver solver_fc(board_cpy);
            solver_fc.set_algorithm(Solver::FC);
            solver_fc.set_heuristic((Solver::H)h);
            f = solver_fc.solve();
            board_cpy.print();
            solver_fc.report(out_file.c_str());
            if (!test(board_cpy)) cout << "WRONG" << endl;
            else cout << "CORRECT" << endl;
        }
    }

    // Board board("csp_task/data/d-15-01.txt.txt");
    
    // Solver solver(board);
    // // solver.set_algorithm(Solver::FC);
    // // solver.set_heuristic(Solver::SDF);

    // bool f = solver.solve();

    // board.print();
    // solver.report();
    // // cout << f << endl;
    // if (!test(board)) cout << "WRONG" << endl;
    return 0;
}
