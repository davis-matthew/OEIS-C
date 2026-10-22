/*
 * A370836  Expansion of (1/x) * Series_Reversion( x/(x+1/(1+x^2)) ).
 * https://oeis.org/A370836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (-1)^k * binomial(n,2k) *
 *            binomial(3k,k) / (2k+1)
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 19, i64 n <= 31, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* multiplicative binomial coefficient: C(n,k) */
static seq_t binomial(idx_t n, idx_t k) {
    if (k > n - k)
        k = n - k;
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* result = result * (n - k + i) / i; */
        result = result * (seq_t)(n - k + i);
        result = result / (seq_t)i;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t k = 0; 2 * k <= n; ++k) {
        idx_t two_k = 2 * k;
        seq_t bin_n_2k = binomial(n, two_k);
        seq_t bin_3k_k = binomial(3 * k, k);
        seq_t term = bin_n_2k * bin_3k_k;
        term = term / (seq_t)(two_k + 1);
        if (k & 1)
            term = -term;
        sum += term;
    }
    return sum;
}
