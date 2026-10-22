/*
 * A331428  Divide each side of a triangle into 2*n-1 (n>=1) equal parts and
 *          trace the corresponding cevians, i.e., join every point, except
 *          for the first and last ones, with the opposite vertex. a(n) is the
 *          number of points at which three cevians meet.
 * https://oeis.org/A331428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A331428
   a(n) = number of ordered triples (a,b,c) with 1 <= a,b,c < 2n-1
   such that a*b*c = (2n-1-a)*(2n-1-b)*(2n-1-c)   */

/* Implementation: iterate all ordered pairs (a,b), solve for
   c = (2n-1)*((2n-1-a)*(2n-1-b))/[a*b + (2n-1-a)*(2n-1-b)].
   If c is integer and 1 <= c < 2n-1 we have a solution. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    idx_t m = 2 * n - 1;           /* m = 2n-1 */
    uint64_t count = 0;           /* 64‑bit counter is enough for n≤10000 */

    for (idx_t a = 1; a < m; ++a)
    {
        idx_t am = m - a;          /* (m-a) */
        for (idx_t b = 1; b < m; ++b)
        {
            idx_t bm = m - b;          /* (m-b) */
            idx_t p   = a * b;         /* a*b   */
            idx_t q   = am * bm;       /* (m-a)(m-b) */
            idx_t sum = p + q;         /* denom for c */
            idx_t numer = q * m;       /* q*m   */

            if (numer % sum != 0)
                continue;

            idx_t c = numer / sum;
            if (c >= 1 && c < m)
                ++count;
        }
    }
    return (seq_t)count;
}
