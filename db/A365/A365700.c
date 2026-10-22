/*
 * A365700  G.f. satisfies A(x) = 1 + x^5*A(x)^3 / (1 - x*A(x)).
 * https://oeis.org/A365700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 41, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A365700
   Formula: a(n) = Σ_{k=0..⌊n/5⌋} C(n-4k-1, n-5k) * C(n-2k+1, k) / (n-2k+1)
   (C(a,b) denotes binomial coefficient) */

#include "../utils/binomial.h"

/* Return the n-th term of A365700 (zero‑based indexing). */
seq_t a(idx_t n)
{
    /* base case: a(0)=1 */
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;

    /* iterate k from 0 to floor(n/5) */
    for (idx_t k = 0; k * 5 <= n; ++k)
    {
        /* Guard against underflow of n-4k-1 */
        if (4 * k + 1 > n)
            break;

        idx_t upper1 = n - 4 * k - 1;   /* n-4k-1 */
        idx_t lower1 = n - 5 * k;       /* n-5k */

        /* If lower1 > upper1 binomial is zero */
        if (lower1 > upper1)
            continue;

        seq_t b1 = binomial(upper1, lower1);
        seq_t b2 = binomial(n - 2 * k + 1, k);

        idx_t denom_idx = n - 2 * k + 1;    /* index for denominator */
        seq_t denom = (seq_t)denom_idx;

        seq_t term = (b1 * b2) / denom;
        sum += term;
    }

    return sum;
}
