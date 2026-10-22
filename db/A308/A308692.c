/*
 * A308692  a(n) = Sum_{d|n} d^(2*(n/d - 1)).
 * https://oeis.org/A308692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Sum_{d|n} d^(2*(n/d - 1))
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 125,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

/* Compute a(n) for n >= 1. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* find the largest integer k such that k*k <= n */
    idx_t limit = 0;
    while ((limit + 1) * (limit + 1) <= n)
        limit++;

    for (idx_t d = 1; d <= limit; ++d)
    {
        if (n % d != 0)
            continue;

        idx_t nd = n / d;          /* complementary divisor */
        seq_t exp1 = (seq_t)(2 * (nd - 1));
        seq_t term1 = big_pow((seq_t)d, exp1);
        sum += term1;

        if (d != nd)
        {
            seq_t exp2 = (seq_t)(2 * (d - 1));
            seq_t term2 = big_pow((seq_t)nd, exp2);
            sum += term2;
        }
    }
    return sum;
}
