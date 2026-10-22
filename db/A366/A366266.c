/*
 * A366266  G.f. A(x) satisfies A(x) = 1 + x + x*A(x)^3.
 * https://oeis.org/A366266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(2*k+1, n-k) * binomial(3*k, k) /
 *            (2*k+1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binom.h"

/* Computes OEIS sequence A366266. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t b1 = binom(2*k + 1, n - k);          /* binomial(2*k+1, n-k) */
        seq_t b2 = binom(3*k, k);                 /* binomial(3*k, k)     */
        seq_t term = b1 * b2;                     /* product of binomials */
        term /= (seq_t)(2*k + 1);                 /* divide by (2*k+1)    */
        sum += term;
    }
    return sum;
}
