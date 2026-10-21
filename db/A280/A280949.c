/*
 * A280949  Expansion of Product_{k>=1} (1 - x^(12*k)) * (1 - x^(4*k-2)) / (1
 *          - x^k).
 * https://oeis.org/A280949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 60, u32 n <= 181, u64 n <= 599,
 *            u128 n <= 2127, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A280949: partitions of n into parts that are not multiples of 12
 * and are not congruent to 2 modulo 4.
 * The generating function is
 *   Π_{k≥1} (1 – x^(12k)) * (1 – x^(4k-2)) / (1 – x^k).
 * The coefficients a(n) are computed by a standard unbounded coin‑change DP.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    size_t N = (size_t)n;                      /* maximum index to compute */
    seq_t *dp = (seq_t *)calloc(N + 1, sizeof(seq_t));  /* zero‑initialized */
    if (!dp) return 0;                         /* allocation check */

    dp[0] = 1;                                 /* empty partition */

    for (size_t w = 1; w <= N; ++w) {          /* each allowed part */
        if (w % 12 == 0 || w % 4 == 2)         /* disallowed */
            continue;

        for (size_t i = w; i <= N; ++i) {      /* add partitions using part w */
            dp[i] += dp[i - w];
        }
    }

    seq_t result = dp[N];
    free(dp);
    return result;
}
