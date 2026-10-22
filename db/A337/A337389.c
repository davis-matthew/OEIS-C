/*
 * A337389  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          column k is the expansion of
 *          sqrt((1+(k-4)*x+sqrt(1-2*(k+4)*x+((k-4)*x)^2)) / (2 *
 *          (1-2*(k+4)*x+((k-4)*x)^2))).
 * https://oeis.org/A337389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = Sum_{j=0..n} k^(n-j) * binom(2*j,j) * binom(2*n,2*j)
 *            The sequence a(n) is the antidiagonal reading of T(n,k): a(n) =
 *            T(i,k) where i = n - d(d+1)/2, k = d - i and d is the unique
 *            integer with d(d+1)/2 ≤ n < (d+1)(d+2)/2
 * Range:     u8 n <= 13, u16 n <= 33, u32 n <= 87, u64 n <= 247,
 *            u128 n <= 731, big any n
 * Helpers:   binom, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binom.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Find diagonal index d. */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n)
        ++d;

    /* Position on that diagonal. */
    idx_t r = n - d * (d + 1) / 2;

    /* Compute row i and column k. */
    idx_t i = r;
    idx_t k = d - r;

    /* Special case: row 0 has all ones. */
    if (i == 0)
        return (seq_t)1;

    /* Summation to compute T(i,k). */
    seq_t sum = 0;
    for (idx_t j = 0; j <= i; ++j) {
        idx_t exp = i - j;                        /* exponent of k */
        seq_t pow_k = seq_pow((seq_t)k, exp);      /* k^(i-j) */
        seq_t b1 = binom(2 * j, j);               /* binom(2j, j) */
        seq_t b2 = binom(2 * i, 2 * j);           /* binom(2i, 2j) */
        seq_t prod = seq_mul(pow_k, seq_mul(b1, b2));
        sum += prod;                              /* addition of seq_t */
    }
    return sum;
}
