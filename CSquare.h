#ifndef _C_SQUARE_H_
#define _C_SQUARE_H_

#include <stdint.h>
#include <iostream>

// Useful enumerations to conveniently address specific squares on the board
enum // Squares
{
    A8 = 91, B8, C8, D8, E8, F8, G8, H8,
    A7 = 81, B7, C7, D7, E7, F7, G7, H7,
    A6 = 71, B6, C6, D6, E6, F6, G6, H6,
    A5 = 61, B5, C5, D5, E5, F5, G5, H5,
    A4 = 51, B4, C4, D4, E4, F4, G4, H4,
    A3 = 41, B3, C3, D3, E3, F3, G3, H3,
    A2 = 31, B2, C2, D2, E2, F2, G2, H2,
    A1 = 21, B1, C1, D1, E1, F1, G1, H1,
};

/***************************************************************
 * declaration of CSquare 
 *
 * This class encapsulates the notion of a square.
 * This class converts between an internal (0 - 119)
 * and a (column,row) representation.
 ***************************************************************/

class CSquare 
{
    public: 
        CSquare(uint32_t square = 0) :
            m_sq(square) {}

        operator int() const { return m_sq; } // Implicit conversion to integer

        int row() const {return (m_sq/10) - 1;} // returns 1 - 8
        int col() const {return (m_sq%10);}     // returns 1 - 8

        friend std::ostream& operator <<(std::ostream &os, const CSquare &rhs)
        {
            char c = (rhs.m_sq%10) + 'a' - 1;
            char r = (rhs.m_sq/10) + '1' - 2;
            return os << c << r;
        }

        int     FromString(const char *s)
        {
            uint32_t col = s[0] - 'a';
            uint32_t row = s[1] - '1';
            m_sq = row*10 + col + 21;
            return (col >= 8 || row >= 8);
            // Returns true if error
        }

    private:
        uint8_t  m_sq; // Internal representation, 0 - 119.
}; /* end of class CSquare */

#endif // _C_SQUARE_H_

