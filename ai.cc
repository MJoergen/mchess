#include <stdlib.h> // exit()

#include "ai.h"
#include "CMoveList.h"

/***************************************************************
 * search
 *
 * This is a very basic alpha-beta search
 ***************************************************************/
int AI::search(int alpha, int beta, int level)
{
    if (level == 0)
        return -m_board.get_value();

    CMoveList moves;
    m_board.find_legal_moves(moves);

    int best_val = -999;

    for (unsigned int i=0; i<moves.size() && best_val < beta; ++i)
    {
        CMove move = moves[i];
        if (move.is_captured_piece_a_king())
        {
            return 999;
        }

        if (best_val > alpha)
        {
            alpha = best_val;
        }

        m_board.make_move(move);

        int num = -search(-beta, -alpha, level-1);
        m_board.undo_move(move);

        if (num > best_val)
        {
            best_val = num;
        }
    }

    return best_val;
} // end of int search

/***************************************************************
 * find_best_move
 *
 * This is the main AI.
 * It returns what it considers to be the best legal move in the
 * current position.
 ***************************************************************/
CMove AI::find_best_move()
{
    CMoveList moves;
    m_board.find_legal_moves(moves);

    std::cout << "info string " << moves.size() << " legal moves." << std::endl;

    std::vector<CMove> best_moves;
    best_moves.reserve(999);
    int best_val = -999;

    for (unsigned int i=0; i<moves.size(); ++i)
    {
        CMove move = moves[i];

        m_board.make_move(move);

        int num = -search(-999, 999, 4);
        m_board.undo_move(move);

        std::cout << "info string " << num << " : " << move << std::endl;

        if (num > best_val)
        {
            best_moves.clear();
        }
        if (num >= best_val)
        {
            best_val = num;
            best_moves.push_back(move);
        }
    }

    if (best_moves.size() == 0)
    {
        std::cout << "BUG: No legal moves!" << std::endl;
        exit(1);
    }

    return best_moves[rand() % best_moves.size()];
} // end of find_best_move

