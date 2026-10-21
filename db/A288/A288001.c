/*
 * A288001  Expansion of 1/((1-x)(1-x^3)(1-x^5) ... (1-x^19)).
 * https://oeis.org/A288001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into odd parts ≤ 19
 * Range:     u8 n <= 29, u16 n <= 88, u32 n <= 416, u64 n <= 5419,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation lazily generates the partition table up to the
   requested index.  It works for both C11 and C++17 with the
   sequence type seq_t (an unsigned integer type or an arbitrary‑precision
   class).  Dynamic allocation is handled via malloc in C and via
   new/delete in C++. */

#include <stdlib.h>

static seq_t *dp = NULL;             /* array of computed values */
static idx_t dp_size = 0;            /* actual size of allocated array (max n + 1) */
static const idx_t parts[10] = {1,3,5,7,9,11,13,15,17,19};

/* Recompute the table up to the given maximum index */
static void recompute(idx_t max_n) {
    idx_t i, p;

    if (dp == NULL) {
        /* First allocation */
#ifdef __cplusplus
        dp = new seq_t[max_n + 1];
#else
        dp = (seq_t *)malloc(sizeof(seq_t) * (size_t)(max_n + 1));
#endif
        dp_size = max_n + 1;
    } else if (dp_size <= max_n + 1) {
        /* Expand the existing array */
#ifdef __cplusplus
        seq_t *new_dp = new seq_t[max_n + 1];
        for (i = 0; i < dp_size; ++i) new_dp[i] = dp[i];
        delete[] dp;
        dp = new_dp;
#else
        seq_t *new_dp = (seq_t *)realloc(dp, sizeof(seq_t) * (size_t)(max_n + 1));
        dp = new_dp;
#endif
        dp_size = max_n + 1;
    }

    /* Initialize to zero (except dp[0]) */
    for (i = 0; i < dp_size; ++i) dp[i] = 0;
    dp[0] = 1;

    /* Classic unbounded knapsack to count partitions into allowed parts */
    for (p = 0; p < 10; ++p) {
        idx_t part = parts[p];
        for (i = part; i <= max_n; ++i) {
            dp[i] += dp[i - part];
        }
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n >= dp_size) recompute(n);
    return dp[n];
}
