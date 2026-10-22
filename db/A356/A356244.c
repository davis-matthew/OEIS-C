/*
 * A356244  a(n) = Sum_{k=1..n} (k-1)^n * Sum_{j=1..floor(n/k)} j^2.
 * https://oeis.org/A356244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (k-1)^n * Sum_{j=1..⌊n/k⌋} j^2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t d = 1; d <= n; ++d)
    {
        seq_t pd = pow_int((seq_t)(d - 1), n);   /* (d‑1)^n */
        if (pd == 0)                    /* skip trivial case when d == 1 */
            continue;
        idx_t max_q = n / d;
        for (idx_t q = 1; q <= max_q; ++q)
        {
            seq_t sq = (seq_t)(q * q);   /* (k/d)^2 = q^2 */
            total += pd * sq;
        }
    }
    return total;
}
