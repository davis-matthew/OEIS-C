/*
 * A305291  Numbers k such that 4*k + 3 is a perfect cube, sorted by absolute
 *          values.
 * https://oeis.org/A305291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 25, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A305291: Numbers k such that 4*k+3 is a perfect cube,
   sorted by absolute value.  The explicit closed form is
   a(n) = 16*m³ + 36*m² + 27*m + 6, where
     m = -(n+1)/2   for odd n
     m =  n/2  - 1   for even n
*/
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset starts at 1 */

    bool negative;
    idx_t m_abs;
    if (n & 1) {                /* odd n */
        m_abs = (n + 1) / 2;
        negative = true;
    } else {                    /* even n */
        m_abs = n / 2;
        if (m_abs == 0) m_abs = 0;  /* n=2 gives m_abs=1-1=0 handled below */
        m_abs -= 1;
        negative = false;
    }

    seq_t m = (seq_t)m_abs;
    if (negative) m = -m;          /* sign the integer */

    /* Compute 16*m^3 + 36*m^2 + 27*m + 6 */
    seq_t span = m * m;            /* m² */
    seq_t cub = span * m;          /* m³ */
    seq_t ans = 16 * cub + 36 * span + 27 * m + 6;
    return ans;
}
