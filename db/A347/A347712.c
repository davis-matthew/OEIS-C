/*
 * A347712  Number of compositions (ordered partitions) of n into at most 5
 *          squares.
 * https://oeis.org/A347712
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..5} A337165(n,k) where A337165(n,k) counts
 *            compositions of n into exactly k squares
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    const idx_t max_k = 5;                /* at most 5 squares */
    static seq_t *dp = NULL;              /* dp[ n*6 + k ] */
    static idx_t dp_max_n = 0;           /* largest n already computed */

    /* Ensure the DP table covers n */
    if (dp == NULL) {
        /* first allocation */
        size_t total = ((size_t)n + 1u) * 6u;
        dp = seq_alloc(total);
        for (size_t i = 0; i < total; ++i) dp[i] = 0;
        dp[0 * 6 + 0] = 1;                /* empty composition of 0 */
        /* compute rows 1..n */
        for (idx_t cur = 1; cur <= n; ++cur) {
            for (idx_t k = 1; k <= max_k; ++k) {
                seq_t sum = 0;
                for (idx_t s = 1; (s * s) <= cur; ++s)
                    sum += dp[(cur - s * s) * 6 + (k - 1)];
                dp[cur * 6 + k] = sum;
            }
        }
        dp_max_n = n;
    } else if (n > dp_max_n) {
        /* grow the table */
        size_t old_cap = ((size_t)dp_max_n + 1u) * 6u;
        size_t new_cap = ((size_t)n + 1u) * 6u;
        dp = seq_realloc(dp, old_cap, new_cap);
        for (size_t i = old_cap; i < new_cap; ++i) dp[i] = 0;

        /* compute new rows only */
        for (idx_t cur = dp_max_n + 1; cur <= n; ++cur) {
            for (idx_t k = 1; k <= max_k; ++k) {
                seq_t sum = 0;
                for (idx_t s = 1; (s * s) <= cur; ++s)
                    sum += dp[(cur - s * s) * 6 + (k - 1)];
                dp[cur * 6 + k] = sum;
            }
        }
        dp_max_n = n;
    }
    /* sum compositions with 0..5 squares */
    seq_t result = 0;
    for (idx_t k = 0; k <= max_k; ++k)
        result += dp[n * 6 + k];
    return result;
}
