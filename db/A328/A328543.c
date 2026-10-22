/*
 * A328543  Number of 5 dots bracelet partitions of n.
 * https://oeis.org/A328543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A328543: Number of 5 dots bracelet partitions of n.
 * Generating function: 
 *   F(q) = \frac{Q(-q,q,\infty)}{Q(q,q,\infty)^{4}\,Q(-q^5,q^5,\infty)}
 *         = \prod_{m\ge1} (1-q^m)^{-4}\;\prod_{\substack{m\ge1\\5\nmid m}}(1+q^m).
 *
 * We compute the coefficient of q^n in this infinite product by dynamic
 * programming.  For each part size m, the factor (1-q^m)^{-4} expands to
 * \sum_{t\ge0} \binom{t+3}{3} q^{t m}.  When m is not a multiple of 5,
 * we also multiply by (1+q^m), effectively adding a copy shifted by one
 * part of size m.  The resulting weight for t copies of m is
 *   w_m(t) = \binom{t+3}{3} + [m\not\equiv0\!\!\pmod5]\binom{t+2}{3}
 *   (t=0 gives w=1).
 *
 * DP: dp[j] = number of ways to obtain weight j using parts < current m.
 * For each m, we convolve dp with the sequence {w_m(t)}.
 *
 * Complexity: O(n^2) time, O(n) space (n up to ~10000).
 * Works for all seq_t types because only +, -, *, / are used.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n) {
    // dp arrays, zero-initialized
    seq_t *dp  = (seq_t *)calloc(n + 1, sizeof(seq_t));
    seq_t *next = (seq_t *)calloc(n + 1, sizeof(seq_t));
    if (!dp || !next) return 0;  // out of memory

    dp[0] = 1;

    for (idx_t m = 1; m <= n; ++m) {
        idx_t tmax = n / m;
        // Precompute weights w[t] for this part size
        seq_t *w = (seq_t *)malloc((tmax + 1) * sizeof(seq_t));
        w[0] = 1;  // base case t=0

        for (idx_t t = 1; t <= tmax; ++t) {
            // base = C(t+3,3)
            seq_t base = (seq_t)(t + 3) * (t + 2) * (t + 1) / 6;
            if (m % 5 == 0) {
                w[t] = base;
            } else {
                // shifted part contributes C(t+2,3)
                seq_t prev = (seq_t)(t + 2) * (t + 1) * t / 6;
                w[t] = base + prev;
            }
        }

        // Convolution: dp -> next
        for (idx_t j = 0; j <= n; ++j) {
            if (dp[j] == 0) continue;
            idx_t max_t = (n - j) / m;
            for (idx_t t = 0; t <= max_t; ++t) {
                next[j + t * m] += dp[j] * w[t];
            }
        }

        // Prepare for next part size
        free(w);
        free(dp);
        dp = next;
        next = (seq_t *)calloc(n + 1, sizeof(seq_t));
        if (!next) return 0;
    }

    seq_t result = dp[n];
    free(dp);
    free(next);
    return result;
}
