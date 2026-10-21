/*
 * A264622  Number of (n+1) X (1+1) arrays of permutations of 0..n*2+1 with
 *          each element having directed index change -2,0 -1,0 0,-1 or 1,1.
 * https://oeis.org/A264622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = a(n-2) + 2*a(n-3) + a(n-4) (for n ≥ 5) Initial values:
 *            a(1)=0, a(2)=1, a(3)=2, a(4)=1, a(5)=1
 * Range:     u8 n <= 18, u16 n <= 34, u32 n <= 65, u64 n <= 127,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#if defined(__cplusplus)
#include <new>
#endif

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset. */
    if (n < 1) {
        return (seq_t)0;
    }

    static seq_t *cache = NULL;      /* 1‑indexed array of cached values */
    static size_t capacity = 0;      /* allocated size of cache (>= n+1) */
    static idx_t max_n = 0;          /* highest n already cached               */

    /* Ensure the cache can hold indices up to n */
    if (capacity < n + 1) {
        size_t needed = n + 1;
        size_t new_cap = capacity ? capacity * 2 : 16;
        if (new_cap < needed)
            new_cap = needed;

#if defined(__cplusplus)
        seq_t *tmp = new (std::nothrow) seq_t[new_cap];
        if (!tmp) return (seq_t)0;      /* allocation failure */
        if (cache) {
            for (size_t i = 0; i < capacity; ++i)
                tmp[i] = cache[i];
            delete [] cache;
        }
        cache = tmp;
        capacity = new_cap;
#else
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;      /* allocation failure */
        cache = tmp;
        capacity = new_cap;
#endif
    }

    /* Seed first five values once (indices 1..5) */
    if (max_n < 5) {
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)2;
        cache[4] = (seq_t)1;
        cache[5] = (seq_t)1;
        max_n = 5;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        cache[i] = cache[i - 3] + (cache[i - 4] * (seq_t)2) + cache[i - 5];
    }
    max_n = n;

    return cache[n];
}
