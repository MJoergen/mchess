#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>

#include "CBoard.h"
#include "ai.h"

/***************************************************************
 * perft
 *
 * Counts the leaf nodes of the legal move tree to the given depth.
 * Useful for testing the move generator. Known values from the
 * start position: 20, 400, 8902, 197281.
 * (Castling and en passant are not implemented, so the numbers
 * diverge from the official ones from depth 5.)
 ***************************************************************/
static unsigned long perft(CBoard &board, int depth)
{
    if (depth == 0)
        return 1;

    CMoveList moves;
    board.find_legal_moves(moves);

    unsigned long count = 0;
    for (unsigned int i=0; i<moves.size(); ++i)
    {
        board.make_move(moves[i]);
        count += perft(board, depth-1);
        board.undo_move(moves[i]);
    }
    return count;
} // end of perft

/***************************************************************
 * main
 *
 * This is where it all starts!
 ***************************************************************/
int main()
{
    srand(time(0)); // Seed the random number generator.

    CBoard board;
    AI ai(board);

    while (true) // Repeat forever
    {
        // Display board
        std::cout << board;

        // Check for end of game
        {
            CMoveList legal;
            board.find_legal_moves(legal);
            if (legal.size() == 0)
            {
                if (board.isKingInCheck())
                    std::cout << "Check-mate!" << std::endl;
                else
                    std::cout << "Stale-mate!" << std::endl;
                return 0;
            }
        }
        std::cout << "Input command : ";

        // Read input from player
        std::string str;
        getline(std::cin, str);
        std::cout << std::endl;

        // Parse input from player
        if (std::cin.eof() || str == "quit")
        {
            exit(0);
        }

        if (str.compare(0, 5, "move ") == 0)
        {
            CMove move;

            if (move.FromString(str.c_str()+5) == NULL)
            {
                // Could not parse move.
                std::cout << "Try again. Use long notation, e.g. e2e4" << std::endl;
                continue; // Go back to beginning
            }

            if (board.IsMoveValid(move))
            {
                std::cout << "You move : " << move << std::endl;
                board.make_move(move);
            }
            else 
            {
                std::cout << "Move " << move << " is not legal." << std::endl;
                continue;
            }
        } // end of "move "

        else if (str == "go")
        {
            CMove best_move = ai.find_best_move();

            std::cout << "bestmove " << best_move << std::endl;

            board.make_move(best_move);
        } // end of "go"

        else if (str.compare(0, 6, "perft ") == 0)
        {
            int depth = atoi(str.c_str()+6);
            std::cout << "perft(" << depth << ") = " << perft(board, depth) << std::endl;
        }

        else if (str == "show")
        {
            CMoveList moves;
            board.find_legal_moves(moves);

                        std::cout << moves << std::endl;
        }

        else 
        {
            std::cout << "Unknown command" << std::endl;
            std::cout << "Valid commands are: quit, move, go, show, perft" << std::endl;
        }
    } // end of while (true)

    return 0;
} // end of int main()

