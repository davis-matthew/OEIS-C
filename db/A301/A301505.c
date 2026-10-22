/*
 * A301505  Expansion of Product_{k>=1} (1 + x^(4*k))*(1 + x^(4*k-1)).
 * https://oeis.org/A301505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct parts congruent
 *            to 0 or 3 modulo 4. The function recomputes the
 *            dynamic‑programming table from scratch whenever a larger index
 *            is requested, keeping the result cached for subsequent calls.
 *            This is sufficient for n ≤ 10 000
 * Range:     u8 n <= 65, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* external types supplied by the build */
extern seq_t a(idx_t);

/* compute A301505 */
seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *dp = NULL;   /* dp[i] = a(i) for i <= max_n */
    static idx_t max_n = 0;   /* highest index computed so far */

    /* allocate or grow cache when necessary */
    if (dp == NULL || n > max_n) {
        /* free old cache if it exists */
        if (dp != NULL) free(dp);

        /* allocate new array */
        dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (dp == NULL) return 0;   /* out of memory */

        /* initialise dp[0] */
        dp[0] = 1;
        for (idx_t i = 1; i <= n; ++i) dp[i] = 0;

        /* dynamic programming over allowed parts */
        for (idx_t k = 1; ; ++k) {
            idx_t p3 = 4 * k - 1;  /* part ≡ 3 (mod 4) */
            idx_t p0 = 4 * k;      /* part ≡ 0 (mod 4) */

            if (p3 > n && p0 > n) break;

            if (p3 <= n) {
                for (idx_t i = n; i >= p3; --i) {
                    dp[i] += dp[i - p3];
                    if (i == p3) break; /* avoid unsigned underflow */
                }
            }
            if (p0 <= n) {
                for (idx_t i = n; i >= p0; --i) {
                    dp[i] += dp[i - p0];
                    if (i == p0) break;
                }
            }
        }
        max_n = n;
    }

    return dp[n];
}
