/*
 * A369481  Expansion of (1/x) * Series_Reversion( x / ((1+x) * (1+x+x^3)) ).
 * https://oeis.org/A369481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..floor(n/3)} binomial(n+1,k) *
 *            binomial(2*n - k + 2, n-3*k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom.h"

/* Computes the OEIS sequence A369481.
   It relies on the helper function binom(idx_t a, idx_t b)
   provided by the build system. */
seq_t a(idx_t n)
{
    /* The sum of terms C(n+1,k) * C(2n-k+2, n-3k). */
    seq_t sum = 0;
    idx_t max_k = n / 3;          /* floor(n/3) */
    for (idx_t k = 0; k <= max_k; ++k)
    {
        /* Compute the two binomials. */
        seq_t c1 = binom(n + 1, k);                         /* C(n+1, k) */
        seq_t c2 = binom(2 * n - k + 2, n - 3 * k);         /* C(2n-k+2, n-3k) */
        sum += c1 * c2;
    }

    /* Divide by n+1 to get the final result. */
    seq_t result = sum / (n + 1);
    return result;
}
