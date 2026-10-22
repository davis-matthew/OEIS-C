/*
 * A367776  a(n) = binomial(2*n, n - 1)*(2*n + 1)! / n!.
 * https://oeis.org/A367776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n, n-1) * (2*n+1)! / n!
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 0-indexed: a(0) = 0, a(1) = 6, a(2) = 240, ...         */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* compute (2*n+1)! / n! = product_{i=n+1}^{2*n+1} i */
    seq_t prod = 1;
    for (idx_t i = n + 1; i <= 2 * n + 1; ++i)
        prod *= i;

    /* binomial(2*n, n-1) */
    seq_t comb = binomial(2 * n, n - 1);

    return comb * prod;
}
