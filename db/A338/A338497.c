/*
 * A338497  Least number of odd cubes needed to represent n.
 * https://oeis.org/A338497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal number of odd cubes summing to n
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* seq_t and idx_t are provided by the build system */

/* Return the least number of odd cubes that sum to n,
   where n ≥ 1. Uses dynamic programming with all odd cubes
   ≤ n. */
seq_t a(idx_t n)
{
    /* Allocate DP array, indices 0..n.  Use calloc so internal
       entries are initially zero; we will override them. */
    seq_t *dp = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!dp)
        return 0;                      /* OOM – return 0 as a safe value */

    /* Base case: 0 needs 0 cubes. */
    dp[0] = 0;

    /* Initialise all other entries to a large sentinel value (n+1). */
    seq_t sentinel = (seq_t)(n + 1);
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = sentinel;

    /* Iterate over all odd cube denominations ≤ n. */
    for (idx_t odd = 1; ; odd += 2) {
        seq_t cube = (seq_t)odd * odd * odd;   /* cube fits in seq_t */
        if (cube > (seq_t)n)
            break;
        idx_t c = (idx_t)cube;                 /* cube fits in idx_t */

        /* Unbounded coin change DP step for this coin. */
        for (idx_t i = c; i <= n; ++i) {
            seq_t cand = dp[i - c] + 1;
            if (cand < dp[i])
                dp[i] = cand;
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
