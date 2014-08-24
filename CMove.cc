#include <ctype.h>

#include "CMove.h"

std::ostream& operator <<(std::ostream &os, const CMove &rhs)
{
    return os << rhs.ToShortString();
} // end of std::ostream& operator <<(std::ostream &os, const CMove &rhs)

