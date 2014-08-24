#ifndef _C_SQUARE_H_
#define _C_SQUARE_H_

#include <stdint.h>
#include <iostream>

/***************************************************************
 * declaration of CSquare 
 ***************************************************************/

class CSquare 
{
  public: 
    CSquare() {}
    ~CSquare() {}

    CSquare(uint32_t col, uint32_t row) :
      m_sq(row*10 + col + 21) {}

    CSquare(uint32_t square) :
      m_sq(square) {}

    operator int() const { return m_sq; }

    bool isValid() const { return 
        (m_sq % 10) >= 1 && (m_sq % 10) <= 8
     && (m_sq / 10) >= 2 && (m_sq / 10) <= 9; }

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
    uint8_t  m_sq;
}; /* end of class CSquare */

#endif // _C_SQUARE_H_

