/*
 * A379022  a(n) = Sum_{k=0..n} binomial(2*n+k-1,k) * binomial(2*n+k,n-k).
 * https://oeis.org/A379022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(2*n+k-1, k) * binomial(2*n+k, n-k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the provided binom() routine. */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    seq_t sum = (seq_t)0;

    for (idx_t k = 0; k <= n; ++k) {
        /* binomial(2*n + k - 1, k) */
        idx_t a1 = 2 * n + k - 1;
        seq_t t1 = binom(a1, k);

        /* binomial(2*n + k, n - k) */
        idx_t a2 = 2 * n + k;
        seq_t t2 = binom(a2, n - k);

        /* product using 128-bit intermediate to avoid overflow on 64-bit builds */
        __uint128_t prod = (__uint128_t)t1 * (__uint128_t)t2;
        sum += (seq_t)prod;
    }

    return sum;
}
