/*
 * A266775  Molien series for invariants of finite Coxeter group D_12
 *          (bisected).
 * https://oeis.org/A266775
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of t^n in 1 / ( (1 - t^6)^2 * ∏_{j=1,
 *            j≠6}^{11} (1 - t^j) ) (i.e. the number of partitions of n using
 *            parts 1..11 with part 6 counted twice)
 * Range:     u8 n <= 15, u16 n <= 44, u32 n <= 178, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp      = NULL;   /* memoisation array for coefficients */
static idx_t dp_cap   = 0;      /* current allocated width of dp   */

/* Computes a(n) using dynamic programming over the part set */
seq_t a(idx_t n)
{
    /* Ensure dp has enough space for indices 0..n */
    if (dp_cap <= n) {
        idx_t new_cap = n + 1;
        size_t sz = (size_t)new_cap * sizeof(seq_t);
        seq_t *tmp = (seq_t *)realloc(dp, sz);
        if (!tmp)  exit(EXIT_FAILURE);
        dp     = tmp;
        dp_cap = new_cap;
    }

    /* Initialise dp[0..n] */
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = 0;

    /* Part set: 1,2,3,4,5,6,6,7,8,9,10,11 (two independent 6's) */
    static const idx_t parts[12] = {
        1, 2, 3, 4, 5, 6, 6, 7, 8, 9, 10, 11
    };

    for (size_t k = 0; k < 12; ++k) {
        idx_t part = parts[k];
        for (idx_t i = part; i <= n; ++i) {
            dp[i] += dp[i - part];
        }
    }

    return dp[n];
}
