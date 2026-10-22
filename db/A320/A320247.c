/*
 * A320247  Expansion of Product_{k=1..16} theta_3(q^k), where theta_3() is
 *          the Jacobi theta function.
 * https://oeis.org/A320247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions to Sum_{k=1..16} k·a_k^2 = n
 *            (i.e. the coefficient of q^n in ∏_{k=1}^{16} θ₃(q^k))
 * Range:     u8 n <= 13, u16 n <= 39, u32 n <= 194, u64 n <= 4617,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Integer square root: floor(sqrt(x)) */
static inline idx_t floor_sqrt(idx_t x) {
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= x) ++r;
    return r;
}

seq_t a(idx_t n) {
    /* Allocate two working arrays of size n+1 */
#ifdef __cplusplus
    seq_t *dp   = new seq_t[n + 1];
    seq_t *next = new seq_t[n + 1];
#else
    seq_t *dp   = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    seq_t *next = (seq_t*)malloc((n + 1) * sizeof(seq_t));
#endif

    /* Initialise dp */
    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;

    /* Process each weight k = 1..16 */
    for (idx_t k = 1; k <= 16; ++k) {
        /* Start with contribution for a = 0 */
        for (idx_t s = 0; s <= n; ++s) next[s] = dp[s];

        idx_t max_a = floor_sqrt(n / k);
        for (idx_t a = 1; a <= max_a; ++a) {
            idx_t t = k * a * a;
            if (t > n) break;
            seq_t weight = 2;   /* non‑zero a gives factor 2 */
            for (idx_t s = t; s <= n; ++s) {
                next[s] += weight * dp[s - t];
            }
        }
        /* Swap pointers for next iteration */
        seq_t *tmp = dp; dp = next; next = tmp;
    }

    seq_t result = dp[n];

    /* Clean up */
#ifdef __cplusplus
    delete[] dp;
    delete[] next;
#else
    free(dp);
    free(next);
#endif

    return result;
}
