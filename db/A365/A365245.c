/*
 * A365245  G.f. satisfies A(x) = 1 + x*A(x)/(1 - x^4*A(x)^3).
 * https://oeis.org/A365245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/4⌋} binomial(n-3*k-1,k) * *
 *            binomial(n-k+1,n-4*k)/(n-k+1)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Handle n = 0 explicitly to avoid negative arguments in binomial. */
    if (n == 0) return 1;

    idx_t limit = n / 4;
    seq_t sum = 0;

    for (idx_t k = 0; k <= limit; ++k) {
        /* First binomial term: C(n-3k-1, k) */
        idx_t a1 = n - 3*k - 1;
        idx_t b1 = k;
        seq_t t1 = binom(a1, b1);

        /* Second binomial term: C(n-k+1, n-4k) */
        idx_t a2 = n - k + 1;
        idx_t b2 = n - 4*k;
        seq_t t2 = binom(a2, b2);

        /* Denominator: (n - k + 1) */
        seq_t denom = a2;

        /* Compute the term and add to the sum. */
        seq_t prod = t1 * t2;
        seq_t term = prod / denom;
        sum += term;
    }
    return sum;
}
