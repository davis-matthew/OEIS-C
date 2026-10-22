/*
 * A365112  G.f. satisfies A(x) = 1 + x / (1 + x*A(x))^5.
 * https://oeis.org/A365112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(n-k) * binomial(n-k+1,k) *
 *            binomial(n+4*k-1,n-k) / (n-k+1) (s = 5 for A365112)
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 13, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

/* Computes the n-th term of OEIS sequence A365112. */
seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    if (n == 0) return 1;

    seq_t sum = 0;

    for (idx_t k = 0; k <= n; ++k)
    {
        idx_t nk = n - k;                   /* n - k */

        /* binomial(nk+1, k) */
        seq_t c1 = binom(nk + 1, k);
        if (c1 == 0) continue;              /* term is zero */

        /* binomial(n+4*k-1, nk) */
        seq_t c2 = binom(n + 4 * k - 1, nk);
        if (c2 == 0) continue;              /* term is zero */

        seq_t term = c1 * c2;
        term /= (seq_t)(nk + 1);             /* divide by (n-k+1) */

        /* multiply by (-1)^(n-k) */
        if (nk & 1U) term = -term;

        sum += term;
    }
    return sum;
}
