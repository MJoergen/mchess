#ifndef _C_MOVE_H_
#define _C_MOVE_H_

#include <sstream>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "CSquare.h"

typedef enum {
  EM = 0,   // Empty
  WP = 1,   // White Pawn
  WN = 2,   // White Knight
  WB = 3,   // White Bishop
  WR = 4,   // White Rook
  WQ = 5,   // White Queen
  WK = 6,   // White King
  BP = -1,  // Black Pawn
  BN = -2,  // Black Knight
  BB = -3,  // Black Bishop
  BR = -4,  // Black Rook
  BQ = -5,  // Black Queen
  BK = -6,  // Black King
  IV = 99   // INVALID
} e_piece;

/***************************************************************
 * declaration of CMove
 ***************************************************************/

class CMove
{
    public:
        friend class CBoard;

        CMove() : m_piece(IV), m_captured(IV) {}

        CMove(const char *str) : m_captured(IV) { FromString(str); }

        CMove(int8_t piece, const CSquare& from, const CSquare& to, int8_t captured = EM,
                int8_t promoted=EM) :
            m_from(from), m_to(to), m_piece(piece), m_captured(captured), m_promoted(promoted) {}

        ~CMove() {}

        friend std::ostream& operator <<(std::ostream &os, const CMove &rhs);

        bool        IsEqual(const CMove &move) const
        {
            if (move.From() != From())
                return false;
            if (move.To() != To())
                return false;
            return true;
        } /* end of IsEqual */

        bool        Valid(void) const
        {
            return m_captured != IV;
        }

        std::string ToShortString() const
        {
            static char pieces[] = "kqrbnp.PNBRQK";

            std::stringstream ss;
            assert (m_captured != IV);

            ss << m_from;
            ss << m_to;

            if (m_promoted != EM)
            {
                ss << (char) tolower(pieces[m_promoted+6]); 
            }
            return ss.str();
        }

        std::string ToLongString() const
        {
            static char pieces[] = "kqrbnp.PNBRQK";

            std::stringstream ss;
            assert (m_captured != IV);

            ss << pieces[m_piece+6];
            ss << m_from;
            if (m_captured != EM)
                ss << "*";
            else
                ss << "-";

            ss << m_to;

            if (m_promoted != EM)
            {
                ss << "=";
                ss << (char) tolower(pieces[m_promoted+6]); 
            }
            return ss.str();
        }

        // Returns NULL if error
        const char * FromString(const char *s)
        { 
            if (m_from.FromString(s) || m_to.FromString(s+2))
                return NULL;

            s += 4;

            m_piece = EM;
            m_captured = EM;
            m_promoted = EM;

            if (m_to.row() == 1)
            {
                switch (tolower(s[0]))
                {
                    case 'q' : m_promoted = BQ; s++; break;
                    case 'r' : m_promoted = BR; s++; break;
                    case 'b' : m_promoted = BB; s++; break;
                    case 'n' : m_promoted = BN; s++; break;
                    case ' ' : break;
                    case '\0': break;
                    default  : return NULL;
                }
            }
            else if (m_to.row() == 8)
            {
                switch (tolower(s[0]))
                {
                    case 'q' : m_promoted = WQ; s++; break;
                    case 'r' : m_promoted = WR; s++; break;
                    case 'b' : m_promoted = WB; s++; break;
                    case 'n' : m_promoted = WN; s++; break;
                    case ' ' : break;
                    case '\0': break;
                    default  : return NULL;
                }
            }

            while (s[0] == ' ')
                s++;

            return s;
        } /* end of FromString */

        CSquare     From(void) const {return m_from;}
        CSquare     To(void) const {return m_to;}
        bool        is_captured_piece_a_king(void) const
        {
            return (m_captured == WK || m_captured == BK);
        }

        bool        is_it_a_capture(void) const
        {
            return (m_captured != EM);
        }

        bool        operator==(const CMove& rhs) const
        {
            return IsEqual(rhs);
        }

    private:
        CSquare     m_from;
        CSquare     m_to;
        int8_t      m_piece;
        int8_t      m_captured;
        int8_t      m_promoted;
}; /* end of CMove */

#endif // _C_MOVE_H_

