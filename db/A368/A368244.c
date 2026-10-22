/*
 * A368244  Number of compositions of n into parts 1, 9, and 10.
 * https://oeis.org/A368244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-9) + a(n-10), a(0) = 1, a(1) = … = a(8) = 1,
 *            a(9) = 2
 * Range:     u8 n <= 27, u16 n <= 51, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Dynamic memoisation for any integral or big‑int seq_t. */

#include <stdlib.h>

static seq_t *cache = NULL;      /* computed values   */
static size_t cache_capacity = 0;/* number of valid entries */

/* Return a(n) for OEIS sequence A368244 */
seq_t a(idx_t n)
{
    /* If already computed, return it. */
    if (cache_capacity > (size_t)n)
        return cache[n];

    /* Grow the cache to size n+1. */
    size_t new_capacity = (size_t)n + 1;
    seq_t *new_cache;

#if defined(__cplusplus)
    /* C++: allocate a new array and copy old values. */
    new_cache = new seq_t[new_capacity];
    if (cache) {
        for (size_t i = 0; i < cache_capacity; ++i)
            new_cache[i] = cache[i];
        delete [] cache;
    }
#else
    /* C: realloc works because seq_t is a POD. */
    new_cache = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
    if (!new_cache) {
        /* Allocation failed – terminate. */
        exit(EXIT_FAILURE);
    }
#endif

    cache = new_cache;
    size_t old_cap = cache_capacity;  /* previous number of entries */
    cache_capacity = new_capacity;    /* update after allocation */

    /* Compute all values up to n. */
    for (idx_t m = old_cap; m <= n; ++m) {
        if (m == 0 || m <= 8)
            cache[m] = (seq_t)1;          /* a(0)=…=a(8)=1 */
        else if (m == 9)
            cache[m] = (seq_t)2;          /* a(9)=2 */
        else
            cache[m] = cache[m-1] + cache[m-9] + cache[m-10];
    }

    return cache[n];
}
