/*
 * A249564  a(n) = Sum_{k = 0..n} (k*(k+1)/2)^n.
 * https://oeis.org/A249564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} (k*(k+1)/2)^n
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Special case: a(0) = 1 */
    if (n == 0)
        return 1;

    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        /* triangular number T_k = k(k+1)/2 */
        seq_t t = ((seq_t)k * ((seq_t)k + 1)) / 2;
        /* t^n in exact arithmetic */
        seq_t p = big_pow(t, (seq_t)n);
        /* accumulate */
        sum += p;
    }
    return sum;
}
