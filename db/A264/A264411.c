/*
 * A264411  a(n) = binomial(2*n^2, n).
 * https://oeis.org/A264411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2*n^2, n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 19,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Compute binomial(n, k) for arbitrary precision seq_t using
 * multiplicative cancellation to keep intermediate values small. */
static seq_t binom_idx(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = (seq_t)n - k + i;          /* numerator */
        seq_t den = (seq_t)i;                  /* denominator */

        /* cancel common factors between num and den */
        seq_t g = gcd(num, den);
        num /= g;
        den /= g;

        /* cancel common factors between current result and den */
        g = gcd(result, den);
        result /= g;
        den /= g;

        /* multiply and then divide by remaining den (exact) */
        result = result * num;
        if (den != 1) result /= den;
    }
    return result;
}

seq_t a(idx_t n) {
    /* 2*n^2 may be large but fits into idx_t for n up to ~10^4 */
    idx_t N = 2 * n * n;
    return binom_idx(N, n);
}
