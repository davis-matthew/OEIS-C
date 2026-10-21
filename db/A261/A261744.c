/*
 * A261744  Number of partitions of n where each part i is marked with a word
 *          of length i over a denary alphabet whose letters appear in
 *          alphabetical order.
 * https://oeis.org/A261744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of ∏_{k≥1} 1/(1 - binom(k+9,9)·x^k)
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the number of partitions of n where each part i is marked
   with a word of length i over digits 0‑9 in non‑decreasing order. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Allocate array for dynamic programming */
    seq_t *dp = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!dp) return 0;          /* out of memory – should not occur */

    dp[0] = 1;                  /* empty partition */

    for (idx_t part = 1; part <= n; ++part) {
        /* Compute weight w = C(part+9, 9) */
        seq_t w = 1;
        for (idx_t i = 1; i <= 9; ++i) {
            w = w * (seq_t)(part + 10 - i);
            w = w / (seq_t)i;
        }

        for (idx_t i = part; i <= n; ++i) {
            dp[i] += w * dp[i - part];
        }
    }

    seq_t res = dp[n];
    free(dp);
    return res;
}
