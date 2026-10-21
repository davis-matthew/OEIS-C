/*
 * A263017  n is the a(n)-th positive integer having its binary weight.
 * https://oeis.org/A263017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   not stated
 * Range:     u8 n <= 546, u16 n <= 9841, u32 n <= 9841, u64 n <= 9841,
 *            u128 n <= 9841, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A263017: a(n) = 1 + #{ m | 0 < m < n, popcount(m)=popcount(n) } */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t res = 0;
    /* popcount of n */
    idx_t w = __builtin_popcountll((unsigned long long)n);

    for (int pos = 63; pos >= 0; --pos)
    {
        if ((n >> pos) & 1)                /* current bit is 1 */
        {
            if ((idx_t)pos >= w)            /* combinations where this bit is 0 */
                res += binomial((idx_t)pos, w);
            w--;                            /* consume this 1 for subsequent bits */
        }
    }
    return res + 1;                         /* rank is count+1 */
}
