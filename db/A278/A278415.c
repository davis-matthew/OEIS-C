/*
 * A278415  a(n) = Sum_{k=0..n} binomial(n, 2k)*binomial(n-k, k)*(-1)^k.
 * https://oeis.org/A278415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} binomial(n, 2k)*binomial(n-k, k)*(-1)^k
 * Range:     i8 n <= 6, i16 n <= 13, i32 n <= 25, i64 n <= 48, i128 n <= 93,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

/* Computes sequence A278415.  binomial(n, k) is provided by the
   utilities and returns a value of type seq_t.  The implementation
   performs the sum directly, using only seq_t arithmetic, so it
   works for the arbitrary‑precision build as well as the fixed
   integer builds. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t kmax = n >> 1;                     /* floor(n/2) */

    for (idx_t k = 0; k <= kmax; ++k) {
        seq_t c1   = binomial(n, (idx_t)(2 * k));
        seq_t c2   = binomial(n - k, k);
        seq_t term = c1 * c2;                /* exact integer multiplication */

        if (k & 1u)                          /* (-1)^k */
            sum -= term;
        else
            sum += term;
    }
    return sum;
}
