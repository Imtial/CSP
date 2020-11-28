#include <iostream>
#include "../include/board.h"
#include "../include/solver.h"
using namespace std;

bool test(Board &board)
{
    int count = 0;
    for (int i = 0; i < board.size(); i++)
    {
        for (int c = 0; c < board.size(); c++)
        {
            count = 0;
            for (int r = 0; r < board.size(); r++)
            {
                if (board.grid[r][c].val == i+1)
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
                if (board.grid[r][c].val == i+1)
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
    cout << "UP & RUNNING" << endl;

    Board board("csp_task/data/d-10-01.txt.txt");
    
    Solver solver(board);

    solver.backtrack();

    board.print();
    solver.report();
    
    return 0;
}
