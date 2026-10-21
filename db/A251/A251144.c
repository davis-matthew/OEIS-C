/*
 * A251144  Number of (n+1) X (3+1) 0..2 arrays with every 2 X 2 subblock
 *          summing to 4 and no 2 X 2 subblock having exactly two nonzero
 *          entries.
 * https://oeis.org/A251144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence:
   a(n) = 3*a(n-1) - 5*a(n-3) + a(n-4) + 2*a(n-5)  (n ≥ 6)
   with initial values
   a(1) = 53, a(2) = 87, a(3) = 143, a(4) = 247, a(5) = 433
   The function memoises results so that calls can come in any order. */

#include <stdlib.h>

/* External types supplied by the build system (do not redeclare) */

static seq_t *cache = NULL;      /* memoisation array (1‑based indexing) */
static idx_t cache_size = 0;     /* highest index stored in cache */

/* Ensure that the cache covers the range up to at least n. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_size)
        return;

    /* Allocate or grow the cache to length n+1 (index 0 unused). */
    size_t new_len = (size_t)(n + 1);
    if (cache == NULL) {
        cache = (seq_t *)calloc(new_len, sizeof(seq_t));
    } else {
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    }

    /* Compute values for the newly added indices. */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i == 1) {
            cache[i] = (seq_t)53;
        } else if (i == 2) {
            cache[i] = (seq_t)87;
        } else if (i == 3) {
            cache[i] = (seq_t)143;
        } else if (i == 4) {
            cache[i] = (seq_t)247;
        } else if (i == 5) {
            cache[i] = (seq_t)433;
        } else {
            /* recurrence: a(i) = 3*a(i-1) - 5*a(i-3) + a(i-4) + 2*a(i-5) */
            seq_t part1 = (seq_t)3 * cache[i - 1];
            seq_t part2 = (seq_t)5 * cache[i - 3];
            seq_t part3 = cache[i - 4];
            seq_t part4 = (seq_t)2 * cache[i - 5];
            cache[i] = part1 - part2 + part3 + part4;
        }
    }

    cache_size = n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    ensure_cache(n);
    return cache[n];
}
