/*
 * A290840  a(n) = n! * [x^n] exp(n*x)/(1 + LambertW(-x)).
 * https://oeis.org/A290840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * n^(n-k) * k^k (OEIS A290840)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;   /* a(0) = 1 */
    /* n > 0 */
    seq_t ans = (seq_t)0;
    seq_t n_big = (seq_t)n;

    /* n^n */
    seq_t n_pow = big_pow(n_big, n);
    seq_t term_npow = n_pow;   /* holds n^{n-k} for current k */

    seq_t bin = (seq_t)1;      /* binomial(n,0) */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t k_big = (seq_t)k;
        seq_t k_pow_k = (k == 0) ? (seq_t)1 : big_pow(k_big, k);

        seq_t term = bin * term_npow;
        term = term * k_pow_k;
        ans += term;

        if (k == n) break;  /* finished */

        /* update binomial coefficient to C(n,k+1) */
        bin = bin * (n - k);
        bin = bin / (k + 1);

        /* update n^{n-k} to n^{n-(k+1)} */
        term_npow = term_npow / n_big;
    }

    return ans;
}
