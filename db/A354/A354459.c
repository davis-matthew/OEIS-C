/*
 * A354459  Lazy cutter's sequence (see Comments).
 * https://oeis.org/A354459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: enumerate reduced proper fractions b/c by antidiagonal sum
   (s = b+c).  For each fraction b/c (with b < c and gcd(b,c)=1), compute
   a = b + c - 1 and return the nth such a.  The order of fractions is
  : s = 3, 4, 5, … ; for odd s: b goes from 1 to ⌊(s-1)/2⌋, for even s:
   b goes from ⌊(s-1)/2⌋ down to 1. */

#include <stdint.h>

static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0)
    {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* n is 1‑indexed; ignore n == 0 */
    if (n == 0)
        return (seq_t)0;

    idx_t count = 0;
    idx_t s = 3;               /* current antidiagonal sum b + c */

    for (;; ++s)
    {
        idx_t a_max = (s - 1) / 2;   /* max numerator with b < c */

        if (s & 1ULL)   /* odd s: process numerators ascending */
        {
            for (idx_t b = 1; b <= a_max; ++b)
            {
                idx_t c = s - b;
                if (gcd_idx(b, c) != 1)
                    continue;

                ++count;
                if (count == n)
                    return (seq_t)(b + c - 1);   /* a = b + c - 1 */
            }
        }
        else            /* even s: numerators descending */
        {
            for (idx_t b = a_max; b; --b)
            {
                idx_t c = s - b;
                if (gcd_idx(b, c) != 1)
                    continue;

                ++count;
                if (count == n)
                    return (seq_t)(b + c - 1);
            }
        }
    }
}
