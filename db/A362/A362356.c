/*
 * A362356  a(n) = 5*(n + 5)^(n-1).
 * https://oeis.org/A362356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*(n+5)^(n-1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   mul_small, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;           /* a(0) = 1 */

    seq_t base   = (seq_t)(n + 5);  /* (n+5) */
    seq_t power  = seq_pow(base, n - 1); /* (n+5)^(n-1) */
    return mul_small(power, 5);    /* 5 * (n+5)^(n-1) */
}
