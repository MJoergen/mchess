#include "CMoveList.h"

std::ostream& operator <<(std::ostream &os, const CMoveList &rhs)
{
    for (std::vector<CMove>::const_iterator it = rhs.m_moveList.begin(); it != rhs.m_moveList.end(); ++it)
    {
        os << (*it).ToShortString() << " ";
    }

    return os;
} // end of std::ostream& operator <<(std::ostream &os, const CMoveList &rhs)

