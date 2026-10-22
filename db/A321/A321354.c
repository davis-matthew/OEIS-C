/*
 * A321354  Expansion of Product_{k>=0} (1 + x^(3^k))^(3^(k+1)).
 * https://oeis.org/A321354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥0} (1 + x^(3^k))^(3^(k+1))
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* compute binomial coefficient C(n, k) in seq_t */
static seq_t binom_seq_t(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;
    }
    return res;
}

/* main sequence function */
seq_t a(idx_t n) {
    /* trivial case */
    if (n == 0) return 1;

    /* dp array holds current partial product coefficients */
    seq_t *dp = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;

    /* iterate over k with 3^k <= n */
    idx_t pow3 = 1;                     /* 3^k */
    while (pow3 <= n) {
        idx_t pk = pow3;                /* 3^k */
        idx_t m = pk * 3;               /* 3^(k+1) */

        idx_t t_max = m;
        idx_t lim = n / pk;
        if (lim < t_max) t_max = lim;  /* t such that t*pk <= n */

        /* binomial coefficients for current factor */
        seq_t *coeff = (seq_t*)malloc((t_max + 1) * sizeof(seq_t));
        coeff[0] = 1;                   /* C(m,0) */
        for (idx_t t = 1; t <= t_max; ++t) {
            coeff[t] = coeff[t - 1] * (m - t + 1) / t;
        }

        /* convolution with current factor */
        seq_t *newdp = (seq_t*)malloc((n + 1) * sizeof(seq_t));
        for (idx_t i = 0; i <= n; ++i) newdp[i] = 0;

        for (idx_t j = 0; j <= n; ++j) {
            seq_t base = dp[j];
            if (base == 0) continue;
            for (idx_t t = 0; t <= t_max; ++t) {
                idx_t exp = t * pk;
                idx_t idx = j + exp;
                if (idx > n) break;
                newdp[idx] += base * coeff[t];
            }
        }

        /* swap dp with newdp */
        free(dp);
        free(coeff);
        dp = newdp;

        pow3 *= 3;   /* next power of 3 */
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
