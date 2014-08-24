#include "CMoveList.h"

std::ostream& operator <<(std::ostream &os, const CMoveList &rhs)
{
    return os << rhs.ToShortString();
} // end of std::ostream& operator <<(std::ostream &os, const CMoveList &rhs)

