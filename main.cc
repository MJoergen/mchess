#include <iostream>
#include <stdlib.h>

#include "CBoard.h"
#include "ai.h"

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
        std::cout << "Input command : ";

        // Read input from player
        std::string str;
        getline(std::cin, str);
        std::cout << std::endl;

        // Parse input from player
        if (std::cin.eof() || str == "quit")
        {
            exit(1);
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
                board.make_move(move);
                bool check = board.isOtherKingInCheck();
                board.undo_move(move);
                if (check)
                {
                    std::cout << "You are in CHECK. Play another move." << std::endl;
                    continue;
                }

                std::cout << "You move : " << move << std::endl;
                board.make_move(move);
            }
            else 
            {
                std::cout << "Move " << move << " is not legal." << std::endl;
                continue;
            }
        } // end of "move "

        else if (str.compare(0, 2, "go") == 0)
        {
            CMove best_move = ai.find_best_move();

            std::cout << "bestmove " << best_move << std::endl;

            board.make_move(best_move);
        } // end of "go"

        else if (str == "show")
        {
            CMoveList moves;
            board.find_legal_moves(moves);

            //            std::cout << moves.ToShortString() << std::endl;
            std::cout << moves << std::endl;
        }

        else 
        {
            std::cout << "Unknown command" << std::endl;
            std::cout << "Valid commands are: quit, move, go, show" << std::endl;
        }
    } // end of while (true)

    return 0;
} // end of int main()

