/*
 * A393988  Expansion of g.f.: Product_{k>=1} 1 / (1 - x^(k*(k+5)/2)).
 * https://oeis.org/A393988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 291, u32 n <= 1010, u64 n <= 4298,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A393988: number of partitions of n into parts of the form k*(k+5)/2.
   Computed via dynamic programming with a static cache that grows
   on demand. */

#include <stddef.h>
#include <stdlib.h>

/* Static cache data */
static seq_t *dp = NULL;
static size_t dp_cap = 0;
static idx_t cur_n = 0;

static idx_t *parts = NULL;
static size_t part_cap = 0;
static size_t part_cnt = 0;
static idx_t last_k = 0;

/* Extend the cache to cover all n up to target_n. */
static void extend_dp(idx_t target_n)
{
    /* Ensure enough space for dp array. */
    if (dp_cap <= target_n) {
        size_t new_cap = dp_cap == 0 ? 8 : dp_cap;
        while (new_cap <= target_n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        if (!tmp) return;               /* allocation failure */
        dp = tmp;
        dp_cap = new_cap;
    }

    /* Re‑initialise dp[0..target_n] to zero, then set dp[0] = 1. */
    for (idx_t i = 1; i <= target_n; ++i) dp[i] = 0;
    dp[0] = 1;

    /* Generate all allowed part sizes up to target_n. */
    for (idx_t k = last_k + 1; ; ++k) {
        idx_t val = k * (k + 5) / 2;
        if (val > target_n) break;
        if (part_cnt == part_cap) {
            size_t new_cap = part_cap == 0 ? 4 : part_cap << 1;
            idx_t *tmp = (idx_t *)realloc(parts, new_cap * sizeof(idx_t));
            if (!tmp) return;
            parts = tmp;
            part_cap = new_cap;
        }
        parts[part_cnt++] = val;
        last_k = k;
    }

    /* Dynamic programming: for each part size, update the table. */
    for (size_t pi = 0; pi < part_cnt; ++pi) {
        idx_t p = parts[pi];
        for (idx_t i = p; i <= target_n; ++i) {
            dp[i] += dp[i - p];
        }
    }

    cur_n = target_n;
}

/* Public interface. */
seq_t a(idx_t n)
{
    /* Lazy initialisation of dp array. */
    if (dp == NULL) {
        dp_cap = 8;
        dp = (seq_t *)calloc(dp_cap, sizeof(seq_t));
        if (!dp) return 0;   /* allocation failure */
        dp[0] = 1;
        cur_n = 0;
    }

    if (n > cur_n) extend_dp(n);
    return dp[n];
}
