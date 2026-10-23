#include <stdlib.h> // exit(), rand()

#include "ai.h"
#include "CMoveList.h"

// Scores are in centipawns. A mate must be worth far more than any
// possible material advantage.
static const int MATE = 30000;
static const int INF  = 32000;

// Number of ply searched below the root move.
static const int SEARCH_DEPTH = 4;

/***************************************************************
 * quiescence
 *
 * Search only captures, until the position is quiet. This avoids
 * evaluating positions in the middle of a capture sequence
 * (the "horizon effect").
 * The side to move may always decline to capture ("stand pat").
 ***************************************************************/
int AI::quiescence(int alpha, int beta)
{
    int stand_pat = m_board.get_value();
    if (stand_pat >= beta)
        return stand_pat;
    if (stand_pat > alpha)
        alpha = stand_pat;

    CMoveList moves;
    m_board.find_pseudo_legal_moves(moves);

    int best_val = stand_pat;

    for (unsigned int i=0; i<moves.size(); ++i)
    {
        const CMove& move = moves[i];
        if (!move.is_it_a_capture())
            continue;

        m_board.make_move(move);
        if (m_board.isOtherKingInCheck())
        {
            // Illegal move
            m_board.undo_move(move);
            continue;
        }
        int num = -quiescence(-beta, -alpha);
        m_board.undo_move(move);

        if (num > best_val)
            best_val = num;
        if (best_val > alpha)
            alpha = best_val;
        if (alpha >= beta)
            break;
    }

    return best_val;
} // end of int quiescence

/***************************************************************
 * search
 *
 * This is a very basic (Fail-Soft Negamax) alpha-beta search.
 * For details, see e.g.
 * http://chessprogramming.wikispaces.com/Alpha-Beta
 * and
 * http://en.wikipedia.org/wiki/Alpha-beta_pruning
 *
 * Parameter values are:
 * * alpha, beta. The search window
 * * level. The number of ply (half-moves) to search.
 *
 * The value returned by this function is
 * * positive, if the side to move has an advantage (is winning).
 * * negative, if the side to move is losing.
 * * zero, if the position is equal.
 * * -(MATE+level), if the side to move is check-mated. A mate that
 *   is found sooner has a larger absolute value.
 * * zero, if the side to move is stale-mated.
 **=************************************************************/
int AI::search(int alpha, int beta, int level)
{
    if (level == 0)
        return quiescence(alpha, beta); // We are at leaf, resolve captures and evaluate.

    CMoveList moves;
    m_board.find_pseudo_legal_moves(moves);

    int best_val = -INF; // Assume the worst
    bool any_legal = false;

    // Search through all pseudo-legal moves
    for (unsigned int i=0; i<moves.size(); ++i)
    {
        const CMove& move = moves[i];

        m_board.make_move(move);
        if (m_board.isOtherKingInCheck())
        {
            // The move left our own king in check, so it is illegal.
            m_board.undo_move(move);
            continue;
        }
        any_legal = true;

        // Do a recursive search
        int num = -search(-beta, -alpha, level-1);
        m_board.undo_move(move);

        if (num > best_val)
        {
            // Store the best value so far.
            best_val = num;
        }
        if (best_val > alpha)
        {
            // Tighten the search window.
            alpha = best_val;
        }
        if (alpha >= beta)
        {
            // This is the alpha-beta pruning.
            // Stop searching, if we already have found a "killer" move.
            break;
        }
    }

    if (!any_legal)
    {
        if (m_board.isKingInCheck())
            return -(MATE + level); // Check-mate
        return 0;                   // Stale-mate
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
    // Make a list of all legal moves
    CMoveList moves;
    m_board.find_legal_moves(moves);

    if (moves.size() == 0)
    {
        std::cout << "BUG: No legal moves!" << std::endl;
        exit(1);
    }

    std::cout << "info string " << moves.size() << " legal moves." << std::endl;

    CMoveList best_moves; // Array of the (one or more) best moves so far
    int best_val = -INF;

    // Search through all legal moves
    for (unsigned int i=0; i<moves.size(); ++i)
    {
        CMove move = moves[i];

        // Get value of current move
        m_board.make_move(move);
        int val = -search(-INF, INF, SEARCH_DEPTH);
        m_board.undo_move(move);

        std::cout << "info string " << val << " : " << move << std::endl;

        if (val > best_val)
        {
            best_val = val;
            best_moves.clear();
            best_moves.push_back(move);
        }
        else if (val == best_val)
        {
            best_moves.push_back(move);
        }
    }

    return best_moves[rand() % best_moves.size()];
} // end of find_best_move

