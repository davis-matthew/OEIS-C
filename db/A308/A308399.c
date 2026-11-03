/*
 * A308399  Expansion of 1 / Sum_{k=-oo..oo} (-x)^(k*(4*k + 1)).
 * https://oeis.org/A308399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts congruent to {0,3,5}
 *            mod 8
 * Range:     u8 n <= 55, u16 n <= 132, u32 n <= 367, u64 n <= 1157,
 *            u128 n <= 3973, big any n
 * Validated: standard, pass, 85 terms through n = 9799 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static storage */
    static seq_t *dp = NULL;
    static size_t dp_size = 0;    /* allocated size (n+1) */

    /* if we need more space or nothing allocated */
    if (n + 1 > dp_size) {
        size_t new_size = (size_t)(n + 1);
        if (dp == NULL) {
            dp = mem_new(seq_t, new_size);
        } else {
            dp = mem_resize(seq_t, dp, new_size);
        }
        /* recompute all coefficients up to n */
        for (size_t i = 0; i < new_size; ++i) {
            dp[i] = (seq_t)0;
        }
        dp[0] = (seq_t)1;
        /* parts ≡ 0 (mod 8) */
        for (idx_t s = 8; s <= n; s += 8) {
            for (idx_t i = s; i <= n; ++i) {
                dp[i] += dp[i - s];
            }
        }
        /* parts ≡ 3 (mod 8) */
        for (idx_t s = 3; s <= n; s += 8) {
            for (idx_t i = s; i <= n; ++i) {
                dp[i] += dp[i - s];
            }
        }
        /* parts ≡ 5 (mod 8) */
        for (idx_t s = 5; s <= n; s += 8) {
            for (idx_t i = s; i <= n; ++i) {
                dp[i] += dp[i - s];
            }
        }
        dp_size = new_size;
    }

    return dp[n];
}
