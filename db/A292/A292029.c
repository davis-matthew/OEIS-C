/*
 * A292029  Wiener index of the n-folded cube graph.
 * https://oeis.org/A292029
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-3) * n * (2^(n-1) - binomial(n-1, floor((n-1)/2)))
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

static seq_t binomial_idx(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;   /* exact division */
    }
    return res;
}

seq_t a(idx_t n) {
    if (n < 2) return 0;            /* undefined below offset, return 0 */
    if (n == 2) return (seq_t)1;    /* a(2) = 1 */

    idx_t k = (n - 1) / 2;          /* floor((n-1)/2) */

    /* 2^(n-3) and 2^(n-1) using big_pow (seq_t base, seq_t exp) */
    seq_t pow2_n_3 = big_pow((seq_t)2, (seq_t)(n - 3));
    seq_t pow2_n_1 = big_pow((seq_t)2, (seq_t)(n - 1));

    seq_t bin = binomial_idx(n - 1, k);
    seq_t diff = pow2_n_1 - bin;

    seq_t result = pow2_n_3 * (seq_t)n;
    result *= diff;
    return result;
}
