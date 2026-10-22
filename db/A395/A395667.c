/*
 * A395667  Expansion of g^8/(7-6*g)^4, where g = 1+x*g^7 is the g.f. of
 *          A002296.
 * https://oeis.org/A395667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 6^k * binomial(k+2,2) * binomial(7*n+7,n-k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>              /* for size_t and NULL */
#include "../utils/binomial.h"   /* provides binomial(total, n) returning seq_t */

/* Computes the n-th term of OEIS sequence A395667.
   n is indexed from 0 (a(0) = 1).  The calculation uses only exact
   integer arithmetic of type seq_t, with intermediate values
   always remaining integers. This method runs in O(n) bigint
   operations and produces the exact value for any n >= 0. */
seq_t a(idx_t n)
{
    /* Total used in all binomials: binom(7*n+7, n-k). */
    idx_t total = 7 * n + 7;

    /* B(k) = binom(total, n-k).  Start with k = 0. */
    seq_t B = binomial(total, n);

    /* C(k) = binom(k+2,2).  Start with k = 0. */
    seq_t C = (seq_t)1;

    /* D(k) = 6^k.  Start with k = 0. */
    seq_t D = (seq_t)1;

    /* Accumulator for the sum. */
    seq_t sum = (seq_t)0;

    for (idx_t k = 0; k <= n; ++k) {
        /* Add current term: 6^k * binom(k+2,2) * binom(7*n+7, n-k). */
        sum += D * C * B;

        if (k == n) break;   /* finished all terms */

        /* Prepare the next values for k+1. */

        /* D_{k+1} = D_k * 6. */
        D *= (seq_t)6;

        /* B_{k+1} = B_k * (n-k) / (total - (n-k) + 1). */
        B *= (seq_t)(n - k);
        B /= (seq_t)(total - (n - k) + 1);

        /* C_{k+1} = C_k * (k+3) / (k+1). */
        C *= (seq_t)(k + 3);
        C /= (seq_t)(k + 1);
    }

    return sum;
}
