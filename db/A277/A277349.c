/*
 * A277349  Expansion of Product_{k>=1} 1/(1 - x^(6*k+1)).
 * https://oeis.org/A277349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts larger than * 1 and
 *            congruent to 1 modulo 6 (i.e., 7, 13, 19, ...)
 * Range:     u8 n <= 149, u16 n <= 339, u32 n <= 900, u64 n <= 2736,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* seq_t and idx_t are provided by the build system.  */
seq_t a(idx_t n)
{
    /* Static cache of the computed sequence up to a certain index. */
    static seq_t *dp = NULL;
    static idx_t dp_max_n = 0;

    /* If the cache is empty or the requested index exceeds the
       current maximum, recompute all values up to n.  */
    if (dp == NULL || n > dp_max_n) {
        /* Free old cache if present. */
        if (dp != NULL)
            free(dp);
        /* Allocate new cache large enough for indices 0..n. */
        dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (dp == NULL) {
            /* Allocation failed; in this context we return 0. */
            return (seq_t)0;
        }
        /* Initialize all entries to 0. */
        for (idx_t i = 0; i <= n; ++i)
            dp[i] = (seq_t)0;
        dp[0] = (seq_t)1;  /* There is one way to partition 0. */

        /* Dynamic programming over allowed part sizes 7,13,19,... */
        for (idx_t p = 7; p <= n; p += 6) {
            for (idx_t i = p; i <= n; ++i) {
                dp[i] = dp[i] + dp[i - p];
            }
        }
        dp_max_n = n;
    }
    return dp[n];
}
