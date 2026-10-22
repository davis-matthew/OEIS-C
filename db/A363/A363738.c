/*
 * A363738  Number of ordered partitions of n into cubes > 1.
 * https://oeis.org/A363738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 157, u16 n <= 278, u32 n <= 514, u64 n <= 978,
 *            u128 n <= 1897, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Number of ordered partitions of n into cubes > 1 (OEIS A363738) \
 * a(n) = 1, if n = 0; \
 * a(n) = Σ_{k: k^3 ≤ n} a(n - k^3), for n ≥ 1   (k ≥ 2)
 *
 * The implementation uses memoised dynamic programming – values are
 * calculated once for every n up to the largest request while keeping
 * a growing array of pre‑computed results.  It works with arbitrary
 * precision seq_t as supplied by the build, and with the built‑in
 * 64‑bit types in the same manner.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *dp = NULL;          /* array of a(i) */
    static idx_t  cap = 0;            /* current allocated capacity */
    static idx_t  last_computed = 0;  /* largest index whose value is known */
    static int    zero_initialized = 0;  /* flag whether a(0) has been set */

    /* Ensure storage for 0 … n is available */
    if (cap <= n) {
        idx_t new_cap = n + 1;           /* need indices up to n inclusive   */
        seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        if (!tmp) abort();              /* allocation failed */
        dp = tmp;
        /* initialise new slots; values will be written later */
        for (idx_t i = cap; i < new_cap; ++i) dp[i] = 0;
        cap = new_cap;
    }

    /* Initialise the base case a(0)=1 once */
    if (!zero_initialized) {
        dp[0] = 1;
        zero_initialized = 1;
        last_computed = 0;
    }

    /* Compute any missing values up to n, in increasing order */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 2;; ++k) {
            idx_t cube = k * k * k;   /* k³, safe for idx_t on all practical n */
            if (cube > i) break;
            sum += dp[i - cube];
        }
        dp[i] = sum;
    }
    last_computed = n;

    return dp[n];
}
