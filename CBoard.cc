#include <assert.h>
#include "CBoard.h"

// Create nice colours in the console output
#define TTY_YELLOW "[33m"
#define TTY_BLUE   "[34m"
#define TTY_NORM   "[0m"

/***************************************************************
 * operator <<
 * This displays the current board configuration on the stream.
 ***************************************************************/
std::ostream& operator <<(std::ostream &os, const CBoard &rhs)
{
    static const char pieces[] = "kqrbnp.PNBRQK";

    for (int row=8; row>=1; --row)
    {
        os << TTY_NORM << row << " ";
        for (int col=1; col<=8; ++col)
        {
            int number = (row+1)*10+col;
            int piece = rhs.m_board[number];
            if (piece != IV)
            {
                if (piece < 0)
                    os << TTY_BLUE;
                else if (piece > 0)
                    os << TTY_YELLOW;
                else
                    os << TTY_NORM;
                os << pieces[piece+6] << " ";
            }
        }
        os << std::endl;
    }
    os << TTY_NORM;
    os << "  a b c d e f g h" << std::endl;
    switch (rhs.m_side_to_move)
    {
        case  1 : os << "White to move" << std::endl; break;
        case -1 : os << "Black to move" << std::endl; break;
    }

    os << std::endl;
    os << "Material : " << rhs.m_material << std::endl;
    if (rhs.isKingInCheck())
    {
        os << "You are in check!" << std::endl;
    }
    return os;
} // end of std::ostream& operator <<(std::ostream &os, const CBoard &rhs)

/***************************************************************
 * newGame
 * This resets the board configuration to the initial game setup
 ***************************************************************/
void CBoard::newGame()
{
    e_piece initial[120] = {
        IV, IV, IV, IV, IV, IV, IV, IV, IV, IV,
        IV, IV, IV, IV, IV, IV, IV, IV, IV, IV,
        IV, WR, WN, WB, WQ, WK, WB, WN, WR, IV,
        IV, WP, WP, WP, WP, WP, WP, WP, WP, IV,
        IV, EM, EM, EM, EM, EM, EM, EM, EM, IV,
        IV, EM, EM, EM, EM, EM, EM, EM, EM, IV,
        IV, EM, EM, EM, EM, EM, EM, EM, EM, IV,
        IV, EM, EM, EM, EM, EM, EM, EM, EM, IV,
        IV, BP, BP, BP, BP, BP, BP, BP, BP, IV,
        IV, BR, BN, BB, BQ, BK, BB, BN, BR, IV,
        IV, IV, IV, IV, IV, IV, IV, IV, IV, IV,
        IV, IV, IV, IV, IV, IV, IV, IV, IV, IV} ;

    m_board.reserve(120);
    for (int i=0; i<120; ++i)
    {
        m_board[i] = initial[i];
    }

    m_side_to_move = 1;
    m_material = 0;
} // end of newGame

/***************************************************************
 * isSquareThreatened
 * Returns true if OTHER side to move threatens this square
 ***************************************************************/
bool CBoard::isSquareThreatened(const CSquare& sq) const
{
    if (m_side_to_move > 0)
    {
        // Check for pawns
        if (m_board[sq + NW] == BP || m_board[sq + NE] == BP)
            return true;

        // Check for knigts
        {
            int dirs[8] = {NNW, NNE, NWW, NEE, SSW, SSE, SWW, SEE};

            for (int k=0; k<8; ++k)
            {
                int dir = dirs[k];
                if (m_board[sq + dir] == BN)
                    return true;
            }
        }

        // Check for diagonal (bishop and queen)
        {
            int dirs[4] = {NW, NE, SW, SE};

            for (int k=0; k<4; ++k)
            {
                int dir = dirs[k];
                int sq2 = sq;
                while (1)
                {
                    sq2 += dir;
                    if (m_board[sq2] == BB || m_board[sq2] == BQ)
                        return true;
                    if (m_board[sq2] != EM)
                        break;
                }

            }
        }

        // Check for horizontal/vertical (rook and queen)
        {
            int dirs[4] = {N, S, W, E};

            for (int k=0; k<4; ++k)
            {
                int dir = dirs[k];
                int sq2 = sq;
                while (1)
                {
                    sq2 += dir;
                    if (m_board[sq2] == BR || m_board[sq2] == BQ)
                        return true;
                    if (m_board[sq2] != EM)
                        break;
                }

            }
        }

        // Check for king
        {
            int dirs[8] = {NW, NE, SW, SE, N, S, W, E};

            for (int k=0; k<8; ++k)
            {
                int dir = dirs[k];
                if (m_board[sq + dir] == BK)
                    return true;
            }
        }
    }
    else
    {
        // Check for pawns
        if (m_board[sq + SW] == WP || m_board[sq + SE] == WP)
            return true;

        // Check for knigts
        {
            int dirs[8] = {NNW, NNE, NWW, NEE, SSW, SSE, SWW, SEE};

            for (int k=0; k<8; ++k)
            {
                int dir = dirs[k];
                if (m_board[sq + dir] == WN)
                    return true;
            }
        }

        // Check for diagonal (bishop and queen)
        {
            int dirs[4] = {NW, NE, SW, SE};

            for (int k=0; k<4; ++k)
            {
                int dir = dirs[k];
                int sq2 = sq;
                while (1)
                {
                    sq2 += dir;
                    if (m_board[sq2] == WB || m_board[sq2] == WQ)
                        return true;
                    if (m_board[sq2] != EM)
                        break;
                }

            }
        }

        // Check for horizontal/vertical (rook and queen)
        {
            int dirs[4] = {N, S, W, E};

            for (int k=0; k<4; ++k)
            {
                int dir = dirs[k];
                int sq2 = sq;
                while (1)
                {
                    sq2 += dir;
                    if (m_board[sq2] == WR || m_board[sq2] == WQ)
                        return true;
                    if (m_board[sq2] != EM)
                        break;
                }

            }
        }

        // Check for king
        {
            int dirs[8] = {NW, NE, SW, SE, N, S, W, E};

            for (int k=0; k<8; ++k)
            {
                int dir = dirs[k];
                if (m_board[sq + dir] == WK)
                    return true;
            }
        }
    }

    return false;
} // end of isSquareThreatened

/***************************************************************
 * find_legal_moves
 * This generates a complete list of all pseudo-legal moves.
 * A pseudo-legal move does not consider, whether the king will
 * be in check. This is handled else-where.
 ***************************************************************/
void CBoard::find_legal_moves(CMoveList &moves) const
{
    moves.clear();

    if (m_side_to_move > 0)
        for (int i=A1; i<=H8; ++i)
        {
            int8_t piece = m_board[i];
            int j;

            switch (piece)
            {
                case WP : // White pawn
                    j = i + 10; // One square forward
                    if (m_board[j] == EM)
                    {
                        if (i > 80) // Check for promotion
                        {
                            {
                                CMove move(piece, i, j, EM, WQ);
                                moves.push_back(move);
                            }
                            {
                                CMove move(piece, i, j, EM, WR);
                                moves.push_back(move);
                            }
                            {
                                CMove move(piece, i, j, EM, WB);
                                moves.push_back(move);
                            }
                            {
                                CMove move(piece, i, j, EM, WN);
                                moves.push_back(move);
                            }
                        }
                        else // regular pawn move
                        {
                            CMove move(piece, i, j, EM);
                            moves.push_back(move);
                        }

                        j = i + 20; // Two squares forward
                        if (m_board[j] == EM)
                        {
                            if (i < 40) // Only from second rank
                            {
                                CMove move(piece, i, j, EM);
                                moves.push_back(move);
                            }
                        }
                    }

                    j = i + 9; // Diagonal capture
                    if ((m_board[j] != IV && m_board[j] < 0))
                    {
                        if (i > 80) // Check for promotion
                        {
                            {
                                CMove move(piece, i, j, m_board[j], WQ);
                                moves.push_back(move);
                            }
                            {
                                CMove move(piece, i, j, m_board[j], WR);
                                moves.push_back(move);
                            }
                            {
                                CMove move(piece, i, j, m_board[j], WB);
                                moves.push_back(move);
                            }
                            {
                                CMove move(piece, i, j, m_board[j], WN);
                                moves.push_back(move);
                            }
                        }
                        else
                        {
                            CMove move(piece, i, j, m_board[j]);
                            moves.push_back(move);
                        }
                    }

                    j = i + 11; // Diagonal capture
                    if ((m_board[j] != IV && m_board[j] < 0))
                    {
                        if (i > 80) // Check for promotion
                        {
                            {
                                CMove move(piece, i, j, m_board[j], WQ);
                                moves.push_back(move);
                            }
                            {
                                CMove move(piece, i, j, m_board[j], WR);
                                moves.push_back(move);
                            }
                            {
                                CMove move(piece, i, j, m_board[j], WB);
                                moves.push_back(move);
                            }
                            {
                                CMove move(piece, i, j, m_board[j], WN);
                                moves.push_back(move);
                            }
                        }
                        else
                        {
                            CMove move(piece, i, j, m_board[j]);
                            moves.push_back(move);
                        }
                    }
                    break;

                case WN : // White knight 
                    {
                        int dirs[8] = {NNW, NNE, NWW, NEE, SSW, SSE, SWW, SEE};

                        for (int k=0; k<8; ++k)
                        {
                            int dir = dirs[k];

                            j = i+dir;
                            if (m_board[j] != IV) 
                                if (m_board[j] <= 0)
                                {
                                    CMove move(piece, i, j, m_board[j]);
                                    moves.push_back(move);
                                }
                        }
                    }

                    break;

                case WB : // White bishop
                    {
                        int dirs[4] = {NE, NW, SE, SW};

                        for (int k=0; k<4; ++k)
                        {
                            int dir = dirs[k];
                            j = i;
                            while (1)
                            {
                                j += dir;
                                if (m_board[j] == IV) break;
                                if (m_board[j] <= 0)
                                {
                                    CMove move(piece, i, j, m_board[j]);
                                    moves.push_back(move);
                                    if (m_board[j] < 0)
                                        break;
                                }
                                else
                                    break;
                            }
                        }
                    }

                    break;

                case WR : // White rook
                    {
                        int dirs[4] = {N, E, S, W};

                        for (int k=0; k<4; ++k)
                        {
                            int dir = dirs[k];
                            j = i;
                            while (1)
                            {
                                j += dir;
                                if (m_board[j] == IV) break;
                                if (m_board[j] <= 0)
                                {
                                    CMove move(piece, i, j, m_board[j]);
                                    moves.push_back(move);
                                    if (m_board[j] < 0)
                                        break;
                                }
                                else
                                    break;
                            }
                        }
                    }

                    break;

                case WQ : // White queen
                    {
                        int dirs[8] = {NE, NW, SE, SW, N, E, S, W};

                        for (int k=0; k<8; ++k)
                        {
                            int dir = dirs[k];
                            j = i;
                            while (1)
                            {
                                j += dir;
                                if (m_board[j] == IV) break;
                                if (m_board[j] <= 0)
                                {
                                    CMove move(piece, i, j, m_board[j]);
                                    moves.push_back(move);
                                    if (m_board[j] < 0)
                                        break;
                                }
                                else
                                    break;
                            }
                        }
                    }

                    break;

                case WK : // White king
                    {
                        int dirs[8] = {NE, NW, SE, SW, N, E, S, W};

                        for (int k=0; k<8; ++k)
                        {
                            int dir = dirs[k];

                            j = i+dir;
                            if (m_board[j] != IV) 
                                if (m_board[j] <= 0)
                                {
                                    CMove move(piece, i, j, m_board[j]);
                                    moves.push_back(move);
                                }
                        }

                    }

                    break;

                default : // Invalid, wrong color, or empty
                    continue;
            }

        }

    else
        for (int i=A1; i<=H8; ++i)
        {
            int8_t piece = m_board[i];
            int j;

            switch (piece)
            {
                case BP : // Black pawn
                    j = i - 10; // One squre forward
                    if (m_board[j] == EM)
                    {
                        if (i < 40) // Check for promotion
                        {
                            {
                                CMove move(piece, i, j, EM, BQ);
                                moves.push_back(move);
                            }
                            {
                                CMove move(piece, i, j, EM, BR);
                                moves.push_back(move);
                            }
                            {
                                CMove move(piece, i, j, EM, BB);
                                moves.push_back(move);
                            }
                            {
                                CMove move(piece, i, j, EM, BN);
                                moves.push_back(move);
                            }
                        }
                        else // Regular pawn move
                        {
                            CMove move(piece, i, j, EM);
                            moves.push_back(move);
                        }

                        j = i - 20; // Two squares forward
                        if (m_board[j] == EM)
                        {
                            if (i > 80) // Only from seventh rank
                            {
                                CMove move(piece, i, j, EM);
                                moves.push_back(move);
                            }
                        }
                    }

                    j = i - 9; // Diagonal capture
                    if ((m_board[j] != IV && m_board[j] > 0))
                    {
                        if (i < 40) // Check for promotion
                        {
                            {
                                CMove move(piece, i, j, m_board[j], BQ);
                                moves.push_back(move);
                            }
                            {
                                CMove move(piece, i, j, m_board[j], BR);
                                moves.push_back(move);
                            }
                            {
                                CMove move(piece, i, j, m_board[j], BB);
                                moves.push_back(move);
                            }
                            {
                                CMove move(piece, i, j, m_board[j], BN);
                                moves.push_back(move);
                            }
                        }
                        else
                        {
                            CMove move(piece, i, j, m_board[j]);
                            moves.push_back(move);
                        }
                    }

                    j = i - 11; // Diagonal capture
                    if ((m_board[j] != IV && m_board[j] > 0))
                    {
                        if (i < 40) // Check for promotion
                        {
                            {
                                CMove move(piece, i, j, m_board[j], BQ);
                                moves.push_back(move);
                            }
                            {
                                CMove move(piece, i, j, m_board[j], BR);
                                moves.push_back(move);
                            }
                            {
                                CMove move(piece, i, j, m_board[j], BB);
                                moves.push_back(move);
                            }
                            {
                                CMove move(piece, i, j, m_board[j], BN);
                                moves.push_back(move);
                            }
                        }
                        else
                        {
                            CMove move(piece, i, j, m_board[j]);
                            moves.push_back(move);
                        }
                    }
                    break;

                case BN : // Black knight
                    {
                        int dirs[8] = {NNW, NNE, NWW, NEE, SSW, SSE, SWW, SEE};

                        for (int k=0; k<8; ++k)
                        {
                            int dir = dirs[k];

                            j = i+dir;
                            if (m_board[j] != IV) 
                                if (m_board[j] >= 0)
                                {
                                    CMove move(piece, i, j, m_board[j]);
                                    moves.push_back(move);
                                }
                        }
                    }

                    break;

                case BB : // Black bishop
                    {
                        int dirs[4] = {NE, NW, SE, SW};

                        for (int k=0; k<4; ++k)
                        {
                            int dir = dirs[k];
                            j = i;
                            while (1)
                            {
                                j += dir;
                                if (m_board[j] == IV) break;
                                if (m_board[j] >= 0)
                                {
                                    CMove move(piece, i, j, m_board[j]);
                                    moves.push_back(move);
                                    if (m_board[j] > 0)
                                        break;
                                }
                                else
                                    break;
                            }
                        }
                    }

                    break;

                case BR : // Black rook
                    {
                        int dirs[4] = {N, E, S, W};

                        for (int k=0; k<4; ++k)
                        {
                            int dir = dirs[k];
                            j = i;
                            while (1)
                            {
                                j += dir;
                                if (m_board[j] == IV) break;
                                if (m_board[j] >= 0)
                                {
                                    CMove move(piece, i, j, m_board[j]);
                                    moves.push_back(move);
                                    if (m_board[j] > 0)
                                        break;
                                }
                                else
                                    break;
                            }
                        }
                    }

                    break;

                case BQ : // Black queen
                    {
                        int dirs[8] = {NE, NW, SE, SW, N, E, S, W};

                        for (int k=0; k<8; ++k)
                        {
                            int dir = dirs[k];
                            j = i;
                            while (1)
                            {
                                j += dir;
                                if (m_board[j] == IV) break;
                                if (m_board[j] >= 0)
                                {
                                    CMove move(piece, i, j, m_board[j]);
                                    moves.push_back(move);
                                    if (m_board[j] > 0)
                                        break;
                                }
                                else
                                    break;
                            }
                        }
                    }

                    break;

                case BK : // Black king
                    {
                        int dirs[8] = {NE, NW, SE, SW, N, E, S, W};

                        for (int k=0; k<8; ++k)
                        {
                            int dir = dirs[k];

                            j = i+dir;
                            if (m_board[j] != IV) 
                                if (m_board[j] >= 0)
                                {
                                    CMove move(piece, i, j, m_board[j]);
                                    moves.push_back(move);
                                }
                        }

                    }

                    break;

                default : // Invalid, wrong color, or empty
                    continue;
            }

        }
} // end of void CBoard::find_legal_moves(CMoveList &moves) const;


/***************************************************************
 * make_move
 * This updates the board according to the move
 ***************************************************************/
void CBoard::make_move(const CMove &move)
{
    switch (move.m_captured)
    {
        case WP : case BP : m_material += 1; break;
        case WN : case BN : m_material += 3; break;
        case WB : case BB : m_material += 3; break;
        case WR : case BR : m_material += 5; break;
        case WQ : case BQ : m_material += 9; break;
        default : break;
    }

    switch (move.m_promoted)
    {
        case WN : case BN : m_material += 3-1; break;
        case WB : case BB : m_material += 3-1; break;
        case WR : case BR : m_material += 5-1; break;
        case WQ : case BQ : m_material += 9-1; break;
        default : break;
    }

    m_board[move.m_to] = m_board[move.m_from];
    if (move.m_promoted != EM)
        m_board[move.m_to] = move.m_promoted;
    m_board[move.m_from] = EM;
    m_side_to_move = -m_side_to_move;
    m_material = -m_material;
} // end of void CBoard::make_move(const CMove &move)


/***************************************************************
 * undo_move
 * This reverses the effect of make_move
 ***************************************************************/
void CBoard::undo_move(const CMove &move)
{
    m_material = -m_material;
    switch (move.m_captured)
    {
        case WP : case BP : m_material -= 1; break;
        case WN : case BN : m_material -= 3; break;
        case WB : case BB : m_material -= 3; break;
        case WR : case BR : m_material -= 5; break;
        case WQ : case BQ : m_material -= 9; break;
        default : break;
    }

    switch (move.m_promoted)
    {
        case WN : case BN : m_material -= 3-1; break;
        case WB : case BB : m_material -= 3-1; break;
        case WR : case BR : m_material -= 5-1; break;
        case WQ : case BQ : m_material -= 9-1; break;
        default : break;
    }

    m_board[move.m_from] = move.m_piece;
    m_board[move.m_to] = move.m_captured;
    m_side_to_move = -m_side_to_move;

} // end of void CBoard::undo_move(const CMove &move)


/***************************************************************
 * IsMoveValid
 * This returns true, if the move is legal.
 ***************************************************************/
bool CBoard::IsMoveValid(CMove &move) const
{
    CMoveList moves;
    find_legal_moves(moves);
    for (unsigned int i=0; i<moves.size(); ++i)
    {
        if (moves[i] == move)
        {
            move.m_piece = m_board[move.m_from];
            move.m_captured = m_board[move.m_to];
            return true;
        }
    }
    return false;
} // end of bool CBoard::IsMoveValid(CMove &move)


/***************************************************************
 * get_value
 *
 * It returns an integer value showing how good the position
 * is for the side to move.
 *
 * This is a very simple evaluation function. Only two factors are
 * considered:
 * 1. The material balance:
 *      * Pawn   100
 *      * Knight 300
 *      * Bishop 300
 *      * Rook   500
 *      * Queen  900
 * 2. The difference in number of legal moves of both players.
 *
 * The latter tends to favor positions, where the computer
 * has many legal moves. This implies centralization and development.
 ***************************************************************/
int CBoard::get_value()
{
    CMoveList moves;
    find_legal_moves(moves);
    int my_moves = moves.size();
    swap_sides();

    find_legal_moves(moves);
    int his_moves = moves.size();
    swap_sides();

    return (my_moves-his_moves) + 100*m_material;
} // end of int CBoard::get_value()


/***************************************************************
 * Returns true if player to move is in check.
 ***************************************************************/
bool CBoard::isKingInCheck() const
{
    e_piece king;
    CSquare kingSquare = 0;

    if (m_side_to_move == 1)
        king = WK;
    else
        king = BK;

    // Look for our king
    for (int i=A1; i<=H8; ++i)
    {
        if (m_board[i] == king)
        {
            kingSquare = i;
            break;
        }
    }

    assert (kingSquare != 0); // The king MUST be somewhere

    return isSquareThreatened(kingSquare);

} // end of isKingInCheck


/***************************************************************
 * Returns true if player NOT to move is in check.
 ***************************************************************/
bool CBoard::isOtherKingInCheck() const
{
    ((CBoard *)this)->m_side_to_move = -m_side_to_move;
    bool retval = isKingInCheck();
    ((CBoard *)this)->m_side_to_move = -m_side_to_move;
    return retval;
} // end of isOtherKingInCheck

