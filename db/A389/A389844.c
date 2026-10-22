/*
 * A389844  E.g.f. A(x) satisfies A(x) = exp(x^2 * A(x) / (1-x)^3).
 * https://oeis.org/A389844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A389844
   Formula: a(n) = n! * Σ_{k=0..⌊n/2⌋} (k+1)^(k-1) * binomial(n+k-1, n-2k) / k!
   (a(0)=1, a(1)=0, ... as listed in the OEIS entry) */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle the special case n = 0 */
    if (n == 0)
        return (seq_t)1;

    /* Compute n! */
    seq_t fact_n = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        fact_n *= (seq_t)i;

    /* prod will hold n! / k! for the current k.  Start with k = 1. */
    seq_t prod = fact_n;
    idx_t max_k = n / 2;

    seq_t sum = (seq_t)0;

    for (idx_t k = 1; k <= max_k; ++k)
    {
        seq_t bin = binomial(n + k - 1, n - 2 * k);
        if (bin != (seq_t)0)
        {
            seq_t pw = big_pow((seq_t)(k + 1), (seq_t)(k - 1));
            seq_t term = bin * pw;
            term = term * prod;   /* prod = n!/k! at this point */
            sum += term;
        }
        /* Prepare prod for the next iteration: divide by (k+1). */
        prod /= (seq_t)(k + 1);
    }

    return sum;
}
