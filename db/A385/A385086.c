/*
 * A385086  a(n) = 2 * (5*n+2)^(n-1).
 * https://oeis.org/A385086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * (5*n + 2)^(n-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;                 /* a(0) = 1 */

    seq_t base = (seq_t)n * (seq_t)5 + (seq_t)2; /* 5*n + 2 */
    seq_t pow_val = seq_pow(base, n - 1);        /* (5*n+2)^(n-1) */
    return seq_mul(pow_val, (seq_t)2);           /* 2 * base^(n-1) */
}
