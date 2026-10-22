/*
 * A376486  G.f. satisfies A(x) = 1 / (1 - x^3*A(x)^3 * (1 + x)).
 * https://oeis.org/A376486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0..⌊n/3⌋} binomial(4*k,k) * binomial(k,n-3*k) /
 *            (3*k+1)
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 29, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>                       /* for bool, if needed by helpers */
#include "../utils/binomial.h"             /* provides seq_t binomial(idx_t, idx_t) */

seq_t a(idx_t n)
{
    /* If n is less than 0 there is no such term, but n is unsigned so
       we simply return 0 for n = 0..otherwise the loop will handle it. */
    seq_t sum = (seq_t)0;

    /* Compute the upper bound for k: floor(n/3). */
    idx_t kmax = n / 3;

    for (idx_t k = (idx_t)0; k <= kmax; ++k)
    {
        /* binomial(4*k, k) */
        idx_t fourk = 4 * k;                      /* fits in idx_t for all k <= kmax  */
        seq_t b1 = binomial(fourk, k);

        /* binomial(k, n-3*k) */
        idx_t rem = n - (3 * k);
        seq_t b2 = binomial(k, rem);

        /* product divided by (3*k+1) */
        seq_t prod = b1 * b2;
        prod /= (seq_t)(3 * k + 1);

        /* accumulate */
        sum += prod;
    }

    return sum;
}
