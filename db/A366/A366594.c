/*
 * A366594  G.f. A(x) satisfies A(x) = 1 + x^3*(1+x)^3*A(x)^4.
 * https://oeis.org/A366594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/3⌋} binomial(3k, n-3k) * binomial(4k, k) /
 *            (3k+1) where binomial(n,k) is the usual binomial coefficient
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The helper function binom is provided by "../utils/binom.h". */
#include <stddef.h>
#include "../utils/binom.h"   /* defines static seq_t binom(idx_t a, idx_t b) */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 3;          /* floor(n/3) */

    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t m = n - 3 * k;          /* second binomial's k argument */
        seq_t c1 = binom(3 * k, m);   /* binomial(3k, n-3k) */
        seq_t c2 = binom(4 * k, k);   /* binomial(4k, k)   */
        seq_t prod = c1 * c2;         /* product of binomials */
        seq_t denom = (seq_t)(3 * k + 1); /* divisor (3k+1) */
        seq_t term = prod / denom;    /* exact division          */
        sum += term;
    }

    return sum;
}
