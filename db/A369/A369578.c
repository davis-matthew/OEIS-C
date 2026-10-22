/*
 * A369578  a(n) = [x^n] Product_{j=1..n, k=1..n} (1 + x^(k^j)).
 * https://oeis.org/A369578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n] ∏_{j=1..n, k=1..n} (1 + x^(k^j))
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 27, u64 n <= 55, u128 n <= 113,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a bounded partition DP and combines the n ones
   via binomial coefficients. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <vector>
#endif

/* The external types seq_t and idx_t are supplied by the build. */
seq_t a(idx_t n) {
    /* Handle the offset. */
    if (n == 0) {
        return (seq_t)1;   /* a(0) = 1 */
    }

    /* Size of arrays.  Use size_t to avoid overflow in C. */
    size_t N = (size_t)n;
    size_t i, j;

    /* Count multiplicities of each exponent e (e >= 2). */
    /* cnt[e] = number of pairs (k,j) with k^j = e,  k,j>=2, e<=n.
       For e==1 we handle separately using the factor (1+x)^n. */
    /* Allocate array of unsigned short (fits up to 65535).  n<=10000. */
    unsigned short *cnt = (unsigned short*)calloc(N + 1, sizeof(unsigned short));
    if (!cnt) return (seq_t)0;  /* allocation failed, return zero */

    for (idx_t k = 2; k <= n; ++k) {
        unsigned long long val = (unsigned long long)k;
        while (val <= n) {
            ++cnt[(size_t)val];
            if (val > (unsigned long long)n / k) break;  /* avoid overflow */
            val *= k;
        }
    }

    /* DP array for sums up to n.  Use dynamic allocation in C and
       std::vector in C++ to properly construct big integers if needed. */
#ifdef __cplusplus
    std::vector<seq_t> dp(N + 1);
    dp[0] = (seq_t)1;
#else
    seq_t *dp = (seq_t*)calloc(N + 1, sizeof(seq_t));
    if (!dp) { free(cnt); return (seq_t)0; }
    dp[0] = (seq_t)1;
#endif

    /* Process all exponents e >= 2.  For each exponent e we have
       cnt[e] copies of the factor (1 + x^e).  We treat each copy
       as a separate bounded knapsack item. */
    for (size_t e = 2; e <= N; ++e) {
        unsigned short m = cnt[e];
        for (unsigned short copy = 0; copy < m; ++copy) {
            for (size_t s = N; s >= e; --s) {
                dp[s] += dp[s - e];
            }
        }
    }

    /* Combine with the n copies of exponent 1 represented by (1+x)^n.
       The coefficient of x^n is sum_{t=0..n} C(n,t) * dp[n-t]. */
    seq_t result = (seq_t)0;
    seq_t binom = (seq_t)1;           /* C(n,0) */
    for (size_t t = 0; t <= N; ++t) {
        result += binom * dp[N - t];
        if (t < N) {
            /* binom = binom * (n - t) / (t + 1)  – integer division exact. */
            binom = (binom * (seq_t)(n - t)) / (seq_t)(t + 1);
        }
    }

    /* Clean up. */
    free(cnt);
#ifdef __cplusplus
    (void)dp;   /* suppress unused variable warning for C++ (vector is auto‑destructed) */
#else
    free(dp);
#endif

    return result;
}
