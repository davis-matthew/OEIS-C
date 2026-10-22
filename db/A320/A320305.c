/*
 * A320305  a(n) = [q^n] Product_{d | n} theta_3(q^d), where theta_3() is the
 *          Jacobi theta function.
 * https://oeis.org/A320305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [q^n] ∏_{d|n} θ₃(q^d) Equivalent combinatorial
 *            interpretation: the number of integer solutions (a_d)_{d|n} to
 *            Σ_{d|n} d·a_d² = n where each non‑zero a_d contributes a factor
 *            2. The function computes this coefficient exactly. For n=0 the
 *            value is 1
 * Range:     u8 n <= 23, u16 n <= 83, u32 n <= 335, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Special case n==0 */
    if (n == 0)
        return (seq_t)1;

    /* ---------- enumerate divisors of n (unsorted) ---------- */
    /* Upper bound on number of divisors for n ≤ 10⁴ is <200,
       but we compute a safe bound dynamically. */
    idx_t div_count = 0;
    idx_t max_divs = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            ++div_count;
            if (i * i != n)
                ++div_count;
        }
    }
    max_divs = div_count;
    idx_t *divs = NULL;
#ifdef __cplusplus
    divs = new idx_t[max_divs];
#else
    divs = malloc(max_divs * sizeof(idx_t));
#endif

    /* Fill divisor list */
    div_count = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            divs[div_count++] = i;
            if (i * i != n)
                divs[div_count++] = n / i;
        }
    }

    /* ---------- dynamic programming arrays ---------- */
    size_t len = (size_t)n + 1U;
#ifdef __cplusplus
    seq_t *dp = new seq_t[len];
    seq_t *newdp = new seq_t[len];
#else
    seq_t *dp = malloc(len * sizeof(seq_t));
    seq_t *newdp = malloc(len * sizeof(seq_t));
#endif
    /* zero initialization */
    for (size_t i = 0; i < len; ++i)
        dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    /* ---------- convolution over each divisor ---------- */
    for (size_t di = 0; di < div_count; ++di) {
        idx_t d = divs[di];

        /* zero newdp */
        for (size_t i = 0; i < len; ++i)
            newdp[i] = (seq_t)0;

        /* weight 0 term: pass through old dp */
        for (size_t i = 0; i < len; ++i)
            newdp[i] += dp[i];

        /* non‑zero weights d·t² */
        for (idx_t t = 1;; ++t) {
            idx_t w = d * t * t;
            if (w > n)
                break;
            seq_t mult = (seq_t)2;   /* coefficient for ±t */
            for (size_t i = (size_t)w; i < len; ++i) {
                newdp[i] += dp[i - (size_t)w] * mult;
            }
        }

        /* swap dp and newdp */
        seq_t *tmp = dp;
        dp = newdp;
        newdp = tmp;
    }

    /* ---------- clean up and return ---------- */
    seq_t result = dp[(size_t)n];
#ifdef __cplusplus
    delete[] dp;
    delete[] newdp;
    delete[] divs;
#else
    free(dp);
    free(newdp);
    free(divs);
#endif
    return result;
}
