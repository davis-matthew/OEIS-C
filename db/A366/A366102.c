/*
 * A366102  Expansion of (1/x) * Series_Reversion( x*(1+x+x^4)/(1+x) ).
 * https://oeis.org/A366102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (1/(n+1)) * Σ_{k=0..⌊n/4⌋} (-1)^(n-k) * binomial(n+k, k)
 *            * binomial(n-3k-1, n-4k)
 * Range:     i8 n <= 9, i16 n <= 17, i32 n <= 32, i64 n <= 37, i128 n <= 37,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
extern "C" {
#endif
#include "../utils/binomial.h"
#ifdef __cplusplus
}
#endif

seq_t a(idx_t n) {
    /* Special case n = 0 (formula gives 0, but a(0) = 1) */
    if (n == 0) {
        return 1;
    }

    seq_t sum = 0;
    idx_t max_k = n / 4;
    for (idx_t k = 0; k <= max_k; ++k) {
        /* first binomial: C(n + k, k) */
        seq_t t1 = binomial(n + k, k);

        /* second binomial: C(n - 3k - 1, n - 4k) */
        idx_t upper = n - 3 * k - 1;
        idx_t lower = n - 4 * k;

        seq_t t2;
        if (upper < lower || upper < 0 || lower < 0) {
            t2 = 0;
        } else {
            t2 = binomial(upper, lower);
        }

        /* combine with sign (-1)^(n-k) */
        seq_t term = t1 * t2;
        if ((n - k) & 1) {
            term = -term;
        }

        sum += term;
    }

    /* divide exactly by n + 1 */
    seq_t divisor = (seq_t)(n + 1);
    return sum / divisor;
}
