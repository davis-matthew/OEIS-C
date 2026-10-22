/*
 * A337481  Number of compositions of n that are neither strictly increasing
 *          nor strictly decreasing.
 * https://oeis.org/A337481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-1) - 2*A000009(n) + 1 (n > 0), a(0) = 0 where
 *            A000009(n) is the number of partitions of n into distinct parts.
 *            The function below computes A000009(n) by dynamic programming
 *            and caches the results up to the largest index seen
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   pow2, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include headers for required utilities */
#include <stddef.h>
#include "../utils/pow2.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Static cache for partitions into distinct parts.
   parts[k] stores A000009(k) for 0 <= k <= max_part. */
static seq_t *parts   = NULL;
static idx_t   max_n   = 0;

/* Recompute partitions up to n (inclusive) using the classic
   distinct-partition DP.  The array is resized if needed. */
static void
ensure_partitions(idx_t n)
{
    /* Allocate or grow the array to hold values up to n. */
    if (!parts) {
        parts = seq_alloc((size_t)n + 1);
        if (!parts) exit(1);
        /* Initial values are zero by seq_alloc. */
        parts[0] = 1;          /* A000009(0) = 1 by convention */
    }
    else if ((size_t)max_n < n) {
        seq_t *tmp = seq_realloc(parts, (size_t)max_n + 1, (size_t)n + 1);
        if (!tmp) exit(1);
        parts = tmp;
        /* New entries are zero by seq_realloc. */
    }

    /* If we already have partitions up to max_n, do nothing. */
    if (n <= max_n) return;

    /* Recompute from scratch for sums up to n.
       This is O(n^2) but n <= 10 000 so it finishes quickly. */
    parts[0] = 1;  /* reset base case */
    for (size_t i = 1; i <= (size_t)n; ++i) parts[i] = 0;

    for (idx_t k = 1; k <= n; ++k) {
        for (idx_t s = n; s >= k; --s) {
            parts[s] += parts[s - k];
        }
    }
    max_n = n;
}

seq_t
a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Compute A000009(n) */
    ensure_partitions(n);
    seq_t pow_term = pow2(n - 1);        /* 2^(n-1) */
    seq_t twoA     = parts[n] << 1;      /* 2 * A000009(n) */
    seq_t res      = pow_term - twoA + (seq_t)1;
    return res;
}
