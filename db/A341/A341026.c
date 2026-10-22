/*
 * A341026  Number of partitions of n into 9 distinct nonzero triangular
 *          numbers.
 * https://oeis.org/A341026
 *
 * Offset:    165    (a(165) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 9 distinct nonzero
 *            triangular numbers
 * Range:     u8 n <= 265, u16 n <= 265, u32 n <= 265, u64 n <= 265,
 *            u128 n <= 265, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds a DP table up to a fixed maximum n (20000). */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t *dp = NULL;          /* DP table stored as 1‑D array */
static bool initialized = false;

#define MAX_C 9
#define MAX_N 20000
#define DPIDX(c,s) ((c)*(MAX_N+1)+(s))

static void init(void)
{
    const idx_t max_n = MAX_N;
    const idx_t max_c = MAX_C;

    dp = (seq_t *)malloc((size_t)(max_c + 1) * (max_n + 1) * sizeof(seq_t));
    if (!dp) return;                 /* allocation failed – keep initialized false */

    /* Zero initialise the DP table. */
    for (size_t i = 0; i < (size_t)(max_c + 1) * (max_n + 1); ++i)
        dp[i] = 0;

    /* Base case: 0 numbers sum to 0. */
    dp[DPIDX(0,0)] = 1;

    /* List all triangular numbers up to max_n. */
    idx_t tri[200];
    size_t tri_cnt = 0;
    for (idx_t k = 1;; ++k) {
        idx_t t = k * (k + 1) / 2;
        if (t > max_n) break;
        tri[tri_cnt++] = t;
    }

    /* Populate the DP table using each triangular number exactly once. */
    for (size_t ti = 0; ti < tri_cnt; ++ti) {
        idx_t t = tri[ti];
        for (int c = max_c - 1; c >= 0; --c) {
            for (idx_t s = max_n - t;; --s) {
                dp[DPIDX(c+1, s + t)] += dp[DPIDX(c, s)];
                if (s == 0) break;
            }
        }
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) init();
    if (!initialized) return 0;      /* if init failed */

    if (n < 165 || n > MAX_N) return 0;   /* out of pre‑computed range */
    return dp[DPIDX(MAX_C, n)];
}
