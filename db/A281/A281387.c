/*
 * A281387  Pairs (x, y) of relatively prime positive integers such that (x^2
 *          - 5)/y and (y^2 - 5)/x are both positive integers.
 * https://oeis.org/A281387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-2) - a(n-4) for n>4, with initial values a(1)=4,
 *            a(2)=11, a(3)=11, a(4)=29
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;   /* store values a[i] for 1 ≤ i ≤ cache_len */
static idx_t cache_len = 0;   /* largest index computed so far */

/* Ensure that cache contains values up to index n (inclusive). */
static void ensure_cache_up_to(idx_t n)
{
    if (n <= cache_len) return;          /* already computed */

    /* Allocate at least 5 elements to hold the first four terms plus a dummy 0 index */
    size_t new_len = (n > 4) ? (size_t)(n + 1) : 5;
    if (!cache) {
        cache = (seq_t *)realloc(NULL, new_len * sizeof(seq_t));
        if (!cache) abort();             /* allocation failure */
        /* initialise first four terms */
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)11;
        cache[3] = (seq_t)11;
        cache[4] = (seq_t)29;
    } else {
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!cache) abort();
    }

    /* compute missing terms using recurrence */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i <= 4) continue;            /* already initialised */
        cache[i] = 3 * cache[i - 2] - cache[i - 4];
    }
    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n == 0) abort();                 /* OEIS offset is 1; n must be ≥1 */
    ensure_cache_up_to(n);
    return cache[n];
}
