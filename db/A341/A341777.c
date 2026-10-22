/*
 * A341777  Number of partitions of n into 6 nonzero tetrahedral numbers.
 * https://oeis.org/A341777
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 6 non‑zero tetrahedral
 *            numbers
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Tetrahedral numbers: T_k = k(k+1)(k+2)/6, k ≥ 1) */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Sequence is defined for n ≥ 6 (offset 6).  For smaller n return 0. */
    if (n < 6U)
        return 0;

    /* ---------------------------------------------------------------------- */
    /* 1. Compute all tetrahedral numbers ≤ n. Store them in an idx_t array.   */
    /* ---------------------------------------------------------------------- */
    idx_t k = 1;
    size_t cap = 16;
    idx_t *tet = (idx_t*)malloc(cap * sizeof(idx_t));
    if (!tet)
        return 0;                    /* out‑of‑memory: return 0 as a fallback */

    size_t cnt = 0;
    for (;; ++k) {
        uint64_t t64 = ((uint64_t)k * (k + 1U) * (k + 2U)) / 6U;
        if (t64 > n)
            break;
        if (cnt >= cap) {            /* grow array when needed */
            cap <<= 1;
            idx_t *tmp = (idx_t*)realloc(tet, cap * sizeof(idx_t));
            if (!tmp) { free(tet); return 0; }
            tet = tmp;
        }
        tet[cnt++] = (idx_t)t64;
    }

    /* ---------------------------------------------------------------------- */
    /* 2. DP table: dp[s][c] = number of ways to form sum s using exactly c  */
    /*    tetrahedral numbers (order irrelevant).  We store it as a flat      */
    /*    array of size (n+1)×7 so that we can index with size_t safely.     */
    /* ---------------------------------------------------------------------- */
    size_t cells = (size_t)(n + 1U) * 7U;
    seq_t *dp = (seq_t*)calloc(cells, sizeof(seq_t));
    if (!dp) { free(tet); return 0; }

    dp[0 * 7U + 0U] = 1;             /* one way to build sum 0 with 0 parts */

    /* ---------------------------------------------------------------------- */
    /* 3. Standard coin‑change DP: iterate over each tetrahedral number.      */
    /*    For each number t, update sums s from t to n (ascending so that     */
    /*    the same t can be used multiple times), and item counts c from 1    */
    /*    to 6.                                                            */
    /* ---------------------------------------------------------------------- */
    for (size_t i = 0; i < cnt; ++i) {
        idx_t t = tet[i];
        for (idx_t s = t; s <= n; ++s) {
            size_t s_idx = (size_t)s * 7U;
            size_t prev_idx = (size_t)(s - t) * 7U;
            for (idx_t c = 1; c <= 6; ++c) {
                dp[s_idx + c] += dp[prev_idx + (c - 1)];
            }
        }
    }

    /* ---------------------------------------------------------------------- */
    /* 4. Result is the number of ways to reach sum n using exactly 6 parts.  */
    /* ---------------------------------------------------------------------- */
    seq_t result = dp[(size_t)n * 7U + 6U];

    /* ---------------------------------------------------------------------- */
    /* 5. Free temporary memory.                                             */
    /* ---------------------------------------------------------------------- */
    free(dp);
    free(tet);
    return result;
}
