/*
 * A383571  Expansion of 1/sqrt((1-x^3)^2 - 4*x^4).
 * https://oeis.org/A383571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(n-2*k,k) *
 *            binomial(k,n-3*k)
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

/* Compute binomial coefficient nCk using the provided binomial helper.
   It returns 0 when k>n. */
static seq_t _binom(idx_t n, idx_t k) {
    if (k > n) return 0;
    return binomial(n, k);
}

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t k = 0; k <= n / 3; ++k) {
        /* First binomial: C(n-2k, k) */
        idx_t n1 = n - 2 * k;
        if (k > n1) continue;                /* invalid */
        /* Second binomial: C(k, n-3k) */
        idx_t k1 = n - 3 * k;
        if (k1 > k) continue;                /* invalid */
        seq_t term1 = _binom(n1, k);
        if (!term1) continue;
        seq_t term2 = _binom(k, k1);
        if (!term2) continue;
        sum += term1 * term2;
    }
    return sum;
}
