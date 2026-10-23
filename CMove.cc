#include <sstream>
#include <cctype>
#include <assert.h>

#include "CMove.h"

std::ostream& operator <<(std::ostream &os, const CMove &rhs)
{
    return os << rhs.ToShortString();
} // end of std::ostream& operator <<(std::ostream &os, const CMove &rhs)

static char pieces[] = "kqrbnp.PNBRQK";

std::string CMove::ToShortString() const
{
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

std::string CMove::ToLongString() const
{
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
// The optional promotion suffix (q, r, b or n) is stored as a white piece
// type here. CBoard::IsMoveValid replaces the whole move with the matching
// generated move, which has the correct colour.
const char * CMove::FromString(const char *s)
{ 
    // Make sure we never read beyond the terminating zero
    for (int i=0; i<4; ++i)
        if (s[i] == '\0')
            return NULL;

    if (m_from.FromString(s) || m_to.FromString(s+2))
        return NULL;

    s += 4;

    m_piece = EM;
    m_captured = EM;
    m_promoted = EM;

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

    while (s[0] == ' ')
        s++;

    return s;
} /* end of FromString */
