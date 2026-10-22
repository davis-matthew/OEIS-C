/*
 * A397116  Size of clusters of star triples of Family C in the Spectre
 *          aperiodic monotile tiling.
 * https://oeis.org/A397116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 15*a(n-2) + a(n-3) + 6, with a(0) = 0, a(1) =
 *            2, a(2) = 12
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* array holding computed values, indexed by n */
static idx_t cache_len = 0;   /* number of entries stored (i.e. cache_len = max_index+1) */

/* Ensure that cache contains values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n < cache_len)            /* already computed */
        return;

    /* Allocate or enlarge array to size n+1 */
    size_t new_len = (size_t)(n + 1ull);
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp && new_len > 0) {     /* allocation failure */
        exit(EXIT_FAILURE);
    }
    cache = tmp;

    /* Initialize known base values if not already present */
    idx_t start = cache_len;       /* next index to compute */

    if (start == 0) {
        cache[0] = (seq_t)0;
        start++;
    }
    if (start == 1) {
        cache[1] = (seq_t)2;
        start++;
    }
    if (start == 2 && new_len > 2) {
        cache[2] = (seq_t)12;
        start++;
    }

    /* Compute remaining values using the recurrence */
    for (idx_t i = start; i <= n; ++i) {
        cache[i] =
            (seq_t)3 * cache[i - 1]          /* 3*a(n-1) */
          + (seq_t)15 * cache[i - 2]         /* 15*a(n-2) */
          + cache[i - 3]                     /* a(n-3) */
          + (seq_t)6;                       /* +6 */
    }

    cache_len = new_len;
}

seq_t a(idx_t n)
{
    if (n == 0)                 /* sequence is defined for n≥1; return 0 for safety */
        return (seq_t)0;
    ensure_cache(n);
    return cache[n];
}
