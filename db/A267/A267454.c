/*
 * A267454  Total number of OFF (white) cells after n iterations of the "Rule
 *          131" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A267454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-3)-a(n-5)-a(n-7)+a(n-8) a(0) = 0, a(1) = 2,
 *            a(2) = 5, a(3) = 10, a(4) = 15, a(5) = 23, a(6) = 30, a(7) = 40
 *            (valid for all n ≥ 0)
 * Range:     u8 n <= 19, u16 n <= 322, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Thread‑unsafe memoisation cache */
static seq_t *cache = NULL;      /* cache[0..cache_len-1]   */
static idx_t cache_cap = 0;      /* allocated elements      */
static idx_t cache_len = 0;      /* computed elements       */

/* Extend the cache so that indices up to n are available
   and compute any missing values using the recurrence. */
static void ensure_cache(idx_t n)
{
    /* Grow capacity if necessary (power‑of‑two growth). */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap < n + 1)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();              /* allocation error */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Nothing to do if we already have the requested value. */
    if (cache_len > n)
        return;

    /* Initialise the first eight values on the first call. */
    if (cache_len == 0) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)10;
        cache[4] = (seq_t)15;
        cache[5] = (seq_t)23;
        cache[6] = (seq_t)30;
        cache[7] = (seq_t)40;
        cache_len = 8;
    }

    /* Compute all remaining indices up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 8)               /* already set above                */
            continue;
        seq_t pos = cache[i-1] + cache[i-3] + cache[i-8];
        seq_t neg = cache[i-5] + cache[i-7];
        cache[i] = pos - neg;
    }

    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
