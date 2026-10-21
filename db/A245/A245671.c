/*
 * A245671  3*x + 1 sequence starting at 1729.
 * https://oeis.org/A245671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 200, u32 n <= 200, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A245671: 3*x + 1 starting at 1729
 * Recurrence:
 *   a(0) = 1729
 *   a(n) = 3*a(n‑1)+1   if a(n‑1) is odd
 *   a(n) = a(n‑1)/2     if a(n‑1) is even
 *
 * The implementation memoises all computed terms to answer
 * arbitrary calls in O(1) average time.
 */

#include <stdlib.h>
#include <stddef.h>

/* Cache of previously computed terms */
static seq_t *seq_cache  = NULL;   /* array of values */
static size_t cache_cap  = 0;      /* current capacity of seq_cache */
static idx_t  cache_last = 0;      /* index of the last computed term */

/* Ensure that seq_cache has at least new_cap entries.
 * Resizes by doubling the capacity until sufficient. */
static void ensure_cache_capacity(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    size_t cap = cache_cap ? cache_cap : 1;
    while (cap < new_cap)
        cap <<= 1;                    /* double until enough */

    /* realloc may return void* in C but needs a cast in C++ */
    void *ptr = realloc(seq_cache, cap * sizeof(seq_t));
    if (!ptr)
        abort();                      /* allocation failed */

    seq_cache = (seq_t *)ptr;
    cache_cap = cap;
}

seq_t a(idx_t n)
{
    /* Initialise the cache on first call */
    if (!seq_cache) {
        ensure_cache_capacity(1);
        seq_cache[0] = (seq_t)1729;
        cache_last  = 0;
    }

    /* If the requested index is already computed, return it */
    if (n <= cache_last)
        return seq_cache[n];

    /* Make sure we have enough room for indices up to n */
    ensure_cache_capacity((size_t)n + 1);

    /* Compute missing terms sequentially */
    for (size_t i = cache_last + 1; i <= (size_t)n; ++i) {
        seq_t prev = seq_cache[i - 1];
        if (prev % 2 == 1)                 /* odd */
            seq_cache[i] = 3 * prev + 1;
        else                               /* even */
            seq_cache[i] = prev / 2;
    }

    cache_last = n;
    return seq_cache[n];
}
