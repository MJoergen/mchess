#ifndef _AI_H_
#define _AI_H_

#include "CBoard.h"

class AI
{
    public:
        AI(CBoard& board) : m_board(board) { }

        CMove find_best_move();

    private:
        int search(int alpha, int beta, int level);

        CBoard&         m_board;
}; // end of class AI

#endif // _AI_H_

