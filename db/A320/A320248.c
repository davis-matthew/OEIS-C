/*
 * A320248  Expansion of Product_{k=1..24} theta_3(q^k), where theta_3() is
 *          the Jacobi theta function.
 * https://oeis.org/A320248
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions of * a1^2 + 2*a2^2 + 3*a3^2 +
 *            … + 24*a24^2 = n * (i.e. the coefficient of q^n in
 *            \prod_{k=1}^{24} \theta_3(q^k)). * The algorithm uses a dynamic
 *            programming convolution over the * quadratic forms; it works for
 *            all non‑negative n in O(24·n·√n) * time and O(n) space
 * Range:     u8 n <= 13, u16 n <= 38, u32 n <= 127, u64 n <= 958,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Count of solutions for a given n. */
seq_t a(idx_t n)
{
    /* Base case: only the all‑zero choice gives sum 0. */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* Allocate the DP array for the current state.  We always use a
       freshly allocated array for the next state to avoid corrupted
       double‑free problems that can arise when swapping pointers. */
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp) return (seq_t)0;          /* allocation failure */

    dp[0] = (seq_t)1;                  /* coefficient for sum 0 */

    /* For each coefficient k = 1..24 we convolve with the series
       1 + 2*q^{k*1^2} + 2*q^{k*2^2} + … .  The factor 2 accounts
       for the ±m solutions when m ≠ 0. */
    for (idx_t k = 1; k <= 24; ++k) {
        seq_t *newdp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
        if (!newdp) {
            free(dp);
            return (seq_t)0;          /* allocation failure */
        }

        for (idx_t m = 0; ; ++m) {
            idx_t m2 = m * m;
            idx_t sq = k * m2;
            if (sq > n)
                break;

            /* Weight: 1 for m=0 (only one choice, a_k=0),
               2 for m>0 (a_k = ±m). */
            if (m == 0) {
                for (idx_t i = 0; i + sq <= n; ++i)
                    newdp[i + sq] += dp[i];
            } else {
                for (idx_t i = 0; i + sq <= n; ++i)
                    newdp[i + sq] += dp[i] + dp[i];
            }
        }

        free(dp);          /* old state no longer needed */
        dp = newdp;        /* proceed with the new state */
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
