/*
 * A284993  Square array A(n,k), n>=0, k>=0, read by antidiagonals, where
 *          column k is the expansion of Product_{j>=1} 1/(1+x^j)^(j^k) in
 *          powers of x.
 * https://oeis.org/A284993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the coefficient of x^i in the product ∏_{j≥1}
 *            1/(1+x^j)^(j^k), where i, k are obtained from n by reading the
 *            square array by antidiagonals
 * Range:     i8 n <= 38, i16 n <= 71, i32 n <= 141, i64 n <= 284,
 *            i128 n <= 542, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* ---------- helper for integer square root (portably) ---------- */
static idx_t isqrt_u64(idx_t x)
{
    idx_t r = 0;
    for (int shift = 62; shift >= 0; shift -= 2) {
        idx_t bit = (idx_t)1 << shift;
        idx_t next = r + bit;
        if ((__int128)next * next <= x)
            r = next;
    }
    return r;
}

/* ---------- binomial coefficient C(N+e-1, e) for small e ---------- */
static seq_t binom_small(seq_t N, int e)
{
    if (e == 0) return (seq_t)1;
    seq_t res = (seq_t)1;
    for (int m = 0; m < e; ++m) {
        res = res * (N + (seq_t)m);
        res = res / (m + 1);   /* exact division */
    }
    return res;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    /* 1. find s such that s*(s+1)/2 <= n < (s+1)*(s+2)/2 */
    idx_t t = (idx_t)(((__int128)8 * n) + 1);
    idx_t sqrt8n1 = isqrt_u64(t);
    idx_t s = (sqrt8n1 - 1) >> 1;            /* floor((sqrt(8n+1)-1)/2) */

    /* adjust in case of rounding error */
    while (((idx_t)(s + 1) * (s + 2) / 2) <= n) ++s;
    while (((idx_t)s * (s + 1) / 2) > n) --s;

    idx_t prev = (idx_t)s * (s + 1) / 2;
    idx_t i = n - prev;                 /* row index */
    idx_t k = s - i;                    /* column index */

    /* 2. compute coefficient of x^i in the product */
    /*  Max i <= s <= ~141 for n <= 10000; use fixed arrays */
    seq_t dp[202] = {0}, newdp[202] = {0}, coeff[202] = {0};
    dp[0] = (seq_t)1;
    for (idx_t t = 1; t <= i; ++t) dp[t] = (seq_t)0;

    for (idx_t j = 1; j <= i; ++j) {
        int maxe = (int)(i / j);

        /* compute coefficients for this j */
        coeff[0] = (seq_t)1;
        seq_t base = (seq_t)j;
        /* compute j^k (big integer) */
        seq_t exp_jk = (seq_t)1;
        for (idx_t c = 0; c < k; ++c) exp_jk *= base;

        for (int e = 1; e <= maxe; ++e) {
            seq_t val = binom_small(exp_jk, e);
            if (e & 1) val = -val;
            coeff[e] = val;
        }

        /* convolution: newdp = dp * coeff (restricted to degree i) */
        for (idx_t t = 0; t <= i; ++t) {
            seq_t sum = (seq_t)0;
            for (int e = 0; e <= maxe && (idx_t)e * j <= t; ++e) {
                sum += dp[t - e * j] * coeff[e];
            }
            newdp[t] = sum;
        }

        /* copy back to dp for next iteration */
        for (idx_t t = 0; t <= i; ++t) dp[t] = newdp[t];
    }

    return dp[i];
}
