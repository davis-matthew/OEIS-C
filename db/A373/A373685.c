/*
 * A373685  Expansion of e.g.f. exp(x / (1 - x^3)^2) / (1 - x^3).
 * https://oeis.org/A373685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=0..⌊n/3⌋} binomial(2*n-5*k, k) / (n-3*k)!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t prod = 1;                     /* product_{i=n-3*k+1}^{n} i , starts at k=0 → 1 */
    idx_t maxk = n / 3;

    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t bin = binomial(2 * n - 5 * k, k);
        sum += prod * bin;

        if (k == maxk)
            break;                     /* no more updates needed */

        /* update prod for next k: multiply by (n-3k-2)*(n-3k-1)*(n-3k) */
        idx_t m1 = n - 3 * k - 2;
        idx_t m2 = n - 3 * k - 1;
        idx_t m3 = n - 3 * k;

        if (m1 > 0)
            prod *= (seq_t)m1;
        if (m2 > 0)
            prod *= (seq_t)m2;
        if (m3 > 0)
            prod *= (seq_t)m3;
    }

    return sum;
}
