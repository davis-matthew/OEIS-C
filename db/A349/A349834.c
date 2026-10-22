/*
 * A349834  Expansion of sqrt(1 + 4*x)/(1 - 4*x).
 * https://oeis.org/A349834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (6*a(n-1) + 8*(2*n-3)*a(n-2)) / n Recurrence: n*a(n) -
 *            6*a(n-1) - 8*(2*n-3)*a(n-2) = 0 Initial values: a(0) = 1, a(1) =
 *            6
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoisation array */
static idx_t cache_len = 0;      /* number of stored entries */

/* Ensure the cache contains values up to index n (inclusive). */
static void grow_cache(idx_t n)
{
    if (n < cache_len)
        return;               /* already available */

    /* Allocate storage for all required indices. */
    size_t new_len = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp)
        abort();              /* out of memory */
    cache = tmp;

    /* Set known initial values if they are not yet set. */
    idx_t start_idx;
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        if (new_len > 1)
            cache[1] = (seq_t)6;
        start_idx = 2;
    } else if (cache_len == 1) {      /* cache[0] present only */
        cache[1] = (seq_t)6;
        start_idx = 2;
    } else {
        start_idx = cache_len;
    }

    /* Compute subsequent terms using the recurrence. */
    for (idx_t i = start_idx; i <= n; ++i) {
        /* Use a wider type for the intermediate arithmetic to avoid
           overflow when seq_t is 64‑bit or less.  The cast to
           unsigned __int128 is permitted by C11/C++17 for any integral
           seq_t; if seq_t is an arbitrary‑precision class, the compile
           infrastructure provides an appropriate conversion.        */
        unsigned __int128 t1 = (unsigned __int128)6 * (unsigned __int128)cache[i - 1];
        unsigned __int128 t2 = (unsigned __int128)8 * ((unsigned __int128)2 * i - 3)
                               * (unsigned __int128)cache[i - 2];
        unsigned __int128 sum = t1 + t2;
        cache[i] = (seq_t)(sum / i);
    }

    cache_len = new_len;
}

seq_t a(idx_t n)
{
    grow_cache(n);
    return cache[n];
}
