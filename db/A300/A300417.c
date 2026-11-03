/*
 * A300417  Expansion of Product_{k>=1} (1 + x^(k*(k+1)/2))^2.
 * https://oeis.org/A300417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in product_{k>=1} (1 +
 *            x^(k*(k+1)/2))^2 (number of partitions of n into distinct
 *            triangular parts, each part available twice)
 * Range:     u8 n <= 58, u16 n <= 242, u32 n <= 1150, u64 n <= 6407,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9882 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    seq_t *dp = mem_new(seq_t, (n + 1));
    if (!dp) return 0;   /* allocation failure – not expected in tests */
    mem_set(dp, 0, (n + 1) * sizeof(seq_t));
    dp[0] = 1;

    for (idx_t k = 1; ; ++k) {
        seq_t tseq = (seq_t)k * ((seq_t)k + 1) / 2;   /* triangular number */
        if (tseq > (seq_t)n) break;
        idx_t t = (idx_t)tseq;

        /* each triangular number yields two identical 0‑1 items of weight t */
        for (int copy = 0; copy < 2; ++copy) {
            for (idx_t j = n; j >= t; --j) {
                dp[j] += dp[j - t];
            }
        }
    }

    seq_t result = dp[n];
    mem_free(dp);
    return result;
}
