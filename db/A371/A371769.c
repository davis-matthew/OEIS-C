/*
 * A371769  a(n) is the smallest natural number k such that every sequence of
 *          length k in Z_n has an A-weighted zero-sum subsequence where A is
 *          the set of all nonzero squares in Z_n.
 * https://oeis.org/A371769
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   if n is a square, a(n)=4 if n even, 5 if n odd; otherwise let
 *            v(n) be the exponent of 2 in n; a(n)=3 if v(n) even, 2 if v(n)
 *            odd
 * Range:     u8 n <= 63, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

static bool is_perfect_square(idx_t n)
{
    idx_t s = (idx_t)floor_sqrt((seq_t)n);
    return ((unsigned __int128)s * s == n);
}

seq_t a(idx_t n)
{
    if (n < 2) return 0; /* undefined below offset, harmless */
    
    if (is_perfect_square(n))
    {
        return (seq_t)((n & 1ULL) ? 5ULL : 4ULL);
    }
    else
    {
        idx_t v = 0;
        idx_t m = n;
        while ((m & 1ULL) == 0)
        {
            v++;
            m >>= 1;
        }
        return (seq_t)((v & 1ULL) ? 2ULL : 3ULL);
    }
}
