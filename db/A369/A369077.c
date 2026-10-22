/*
 * A369077  Expansion of (1/x) * Series_Reversion( x * (1+x^3/(1-x))^2 ).
 * https://oeis.org/A369077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (1/(n+1)) * Σ_{k=0..⌊n/3⌋} (-1)^k * binomial(2*n+k+1, k)
 *            * binomial(n-2*k-1, n-3*k)
 * Range:     i8 n <= 5, i16 n <= 11, i32 n <= 23, i64 n <= 31, i128 n <= 31,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    idx_t k_max = n / 3;
    seq_t sum = 0;
    for (idx_t k = 0; k <= k_max; ++k) {
        /* term1 = binomial(2*n + k + 1, k) */
        idx_t a1 = 2*n + k + 1;
        seq_t term1 = binomial(a1, k);

        /* term2: binomial(n-2*k-1, n-3*k) with careful handling of negatives */
        seq_t term2;
        if (n == 0 && k == 0) {
            term2 = 1;            /* binomial(-1, 0) is defined as 1 in the formula */
        } else {
            idx_t low = (n >= 2*k + 1) ? n - 2*k - 1 : 0xFFFFFFFFFFFFFFFF; /* underflow guard */
            idx_t high = (n >= 3*k)     ? n - 3*k     : 0xFFFFFFFFFFFFFFFF;
            if (low == 0xFFFFFFFFFFFFFFFF || high > low) {
                term2 = 0;            /* invalid or upper > lower → 0 */
            } else {
                term2 = binomial(low, high);
            }
        }

        seq_t prod = term1 * term2;
        if (k & 1)
            sum -= prod;   /* (-1)^k */
        else
            sum += prod;
    }

    seq_t denom = (seq_t)(n + 1);
    return sum / denom;
}
