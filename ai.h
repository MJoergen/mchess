#ifndef AI_H_
#define AI_H_

#include "CBoard.h"

class AI
{
    public:
        AI(CBoard& board) : m_board(board) { }

        CMove find_best_move();

    private:
        int search(int alpha, int beta, int level);
        int quiescence(int alpha, int beta);

        CBoard&         m_board;
}; // end of class AI

#endif // AI_H_

