/*
 * A382449  Expansion of e.g.f. exp( x/(1-2*x)^(3/2) ).
 * https://oeis.org/A382449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * Sum_{k=0..n} 2^(n-k) * binomial(n + k/2-1, n-k)/k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Return the n-th term of the OEIS sequence A382449. */
seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t binom = 1; /* C(n,0) */
    for (idx_t k = 0; k <= n; ++k)
    {
        idx_t m = n - k;              /* m = n-k */
        seq_t prod = 1;
        for (idx_t i = 1; i <= m; ++i)
        {
            seq_t term = (seq_t)(2 * n + k - 2 * i); /* term may be negative only for n=0, handled by prod=1 */
            prod = prod * term;
        }
        seq_t term = binom * prod;
        result += term;
        if (k < n)
        {
            /* Next binomial coefficient: C(n,k+1) = C(n,k) * (n-k)/(k+1) */
            seq_t numerator = binom * (n - k);
            binom = numerator / (k + 1);
        }
    }
    return result;
}
