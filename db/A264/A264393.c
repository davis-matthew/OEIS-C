/*
 * A264393  Number of partitions of n having no perfect cube parts (n>=0).
 * https://oeis.org/A264393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts that are not *
 *            perfect cubes (OEIS A264393). The implementation uses a classic
 *            * coin-change dynamic programming approach; the partition values
 *            are * stored in a static array and recomputed from scratch
 *            whenever a * larger n is requested. The computatio...
 * Range:     u8 n <= 23, u16 n <= 57, u32 n <= 158, u64 n <= 489,
 *            u128 n <= 1638, big any n
 * Helpers:   int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/int_cube_root.h"

static seq_t *dp = NULL;     /* cached partition counts */
static idx_t max_n = 0;      /* largest n for which dp is valid */

/* Return the number of partitions of n with no perfect cube parts. */
seq_t a(idx_t n)
{
    /* Allocate or upscale the cache if needed. */
    if (dp == NULL || n > max_n) {
        idx_t new_n = n;

        /* Allocate new array (may replace a previous one). */
        seq_t *new_dp = (seq_t *)malloc((new_n + 1) * sizeof(seq_t));
        if (new_dp == NULL) { /* allocation failure unlikely for n<=10000 */
            abort();
        }

        /* Base case: partition of 0. */
        new_dp[0] = 1;

        /* Initialize all other entries to 0. */
        for (idx_t i = 1; i <= new_n; ++i)
            new_dp[i] = 0;

        /* Dynamic programming over allowed parts. */
        for (idx_t part = 2; part <= new_n; ++part) {
            idx_t r = int_cube_root(part);
            if (r * r * r == part)          /* skip perfect cubes */
                continue;

            for (idx_t sum = part; sum <= new_n; ++sum) {
                new_dp[sum] += new_dp[sum - part];
            }
        }

        /* Replace old cache. */
        free(dp);
        dp = new_dp;
        max_n = new_n;
    }

    return dp[n];
}
