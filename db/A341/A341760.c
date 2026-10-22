/*
 * A341760  Least k > 1 such that (n^k+k)/(n+k) is an integer.
 * https://oeis.org/A341760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 861, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow_idx.h"
/* Computes the least k > 1 such that (n^k + k)/(n + k) is an integer
   (OEIS A341760).  The algorithm simply tests successive k by
   checking the divisibility condition using modular exponentiation. */

/* Main function requested by the build. */
seq_t a(idx_t n)
{
    /* Convert index to idx_t (already is) for arithmetic. */
    idx_t nn = n;

    for (idx_t k = 2; ; ++k)
    {
        idx_t mod = nn + k;          /* n + k   (always ≥ 2) */
        idx_t p   = mod_pow_idx(nn, k, mod); /* n^k mod (n+k) */

        /* Check the divisibility condition. */
        if ( (p + k) % mod == 0 )
            return (seq_t)k;        /* cast to the sequence type */
    }
}
