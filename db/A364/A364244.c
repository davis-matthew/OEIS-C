/*
 * A364244  a(n) = A143007(2*n-1, n-1) for n >= 1.
 * https://oeis.org/A364244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k = 0..n-1} binomial(2*n-1, k)^2 * binomial(3*n-2-k,
 *            2*n-1)^2 Computation uses multiplicative updates of the two
 *            binomials to obtain an O(n) routine for all n >= 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binomial.h"

/* Return the nth term of A364244 (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* not used by OEIS, but keeps function well‑defined */

    /* N = 2*n-1  (upper index of the first binomial)
       M = 3*n-2  (upper index of the second binomial) */
    idx_t N = 2 * n - 1;
    idx_t M = 3 * n - 2;

    seq_t sum = 0;                /* accumulator for the sum */

    /* c1 holds binomial(N, k) and c2 holds binomial(M - k, N).  We begin
       with k = 0. */
    seq_t c1 = 1;
    seq_t c2 = binomial(M, N);

    for (idx_t k = 0; k < n; ++k)
    {
        /* term = (c1 * c2)^2 */
        seq_t prod = c1 * c2;
        seq_t term = prod * prod;
        sum += term;

        if (k + 1 < n)  /* update for next k */
        {
            /* Update c1: binomial(N, k+1) = binomial(N, k) * (N-k) / (k+1) */
            c1 = c1 * (N - k) / (k + 1);

            /* Update c2: binomial(M-k-1, N) =
               binomial(M-k, N) * ( (M-k) - N ) / (M-k) */
            idx_t denom = M - k;
            idx_t numer = denom - N;   /* denom - N == (M - k) - N */
            c2 = c2 * numer / denom;
        }
    }

    return sum;
}
