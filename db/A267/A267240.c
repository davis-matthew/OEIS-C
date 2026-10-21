/*
 * A267240  Number of n X 3 binary arrays with row sums nondecreasing and
 *          columns lexicographically nondecreasing.
 * https://oeis.org/A267240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 37, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A267240: Number of n×3 binary arrays with row sums nondecreasing
 *  and columns lexicographically nondecreasing.
 *
 *  Recurrence (valid for n ≥ 7):
 *     a(n) = 10*a(n-1) - 39*a(n-2) + 76*a(n-3) - 79*a(n-4)
 *           + 42*a(n-5) - 9*a(n-6)
 *  Initial values (1‑indexed):
 *     a(1)=4, a(2)=13, a(3)=42, a(4)=141,
 *     a(5)=486, a(6)=1685
 *
 *  The function caches values so subsequent calls are O(1).  All
 *  arithmetic uses the provided seq_t type so it works for the
 *  arbitrary‑precision build as well as fixed‑width integer builds.
 */

#include <stddef.h>
#include <stdlib.h>

/* Ensure that *arr has at least "needed" elements, growing if necessary. */
static void ensure_capacity(size_t needed, size_t *cap, seq_t **arr)
{
    if (needed <= *cap) return;
    size_t new_cap = *cap ? *cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    /* n is 1‑based for this sequence; 0 or negative values are undefined. */
    if (n == 0) return (seq_t)0;

    static seq_t *cache = NULL;
    static size_t cache_size = 0;    /* highest index that has been computed */
    static size_t cache_cap  = 0;

    /* Allocate space for indices 0 … n (index 0 unused). */
    ensure_capacity((size_t)n + 1, &cache_cap, &cache);

    /* Initialise the cache on first call. */
    if (cache_size == 0) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)13;
        cache[3] = (seq_t)42;
        cache[4] = (seq_t)141;
        cache[5] = (seq_t)486;
        cache[6] = (seq_t)1685;
        cache_size = 6;
    }

    /* Return the value if it has already been computed. */
    if ((size_t)n <= cache_size) {
        return cache[(size_t)n];
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t val = (seq_t)0;
        val = val + cache[(size_t)(i - 1)] * (seq_t)10;
        val = val - cache[(size_t)(i - 2)] * (seq_t)39;
        val = val + cache[(size_t)(i - 3)] * (seq_t)76;
        val = val - cache[(size_t)(i - 4)] * (seq_t)79;
        val = val + cache[(size_t)(i - 5)] * (seq_t)42;
        val = val - cache[(size_t)(i - 6)] * (seq_t)9;
        cache[(size_t)i] = val;
    }
    cache_size = (size_t)n;
    return cache[(size_t)n];
}
