/*
 * A304621  a(n) = 10 - n for 1 <= n <= 9. Thereafter a(n) = a(n-a(n-3)) +
 *          a(n-a(n-6)).
 * https://oeis.org/A304621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(1)=9,a(2)=8,a(3)=7,a(4)=6,a(5)=5,a(6)=4,a(7)=3,a(8)=2,a(9)=1;
 *            a(n)=a(n-a(n-3))+a(n-a(n-6)) for n>9 (OEIS A304621)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* The function must be externally visible. */
seq_t a(idx_t n)
{
    /* On the first call, allocate a small cache.  The array is 1‑based;
       cache[1] holds a(1), cache[2] holds a(2), etc. */
    static seq_t *cache   = NULL;   /* cache[1..cached_max] filled */
    static idx_t  cached_max = 0;   /* largest n whose value is cached */
    static size_t cache_cap  = 0;   /* current allocated capacity (>=n+1) */

    if (n == 0)                /* offset starts at 1, undefined for 0 */
        return (seq_t)0;

    /* Ensure we have room for index n.  The capacity is kept at powers
       of two for amortised linear growth. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;
#if defined(__cplusplus)
        cache = reinterpret_cast<seq_t*>(realloc(reinterpret_cast<seq_t*>(cache),
                                                  new_cap * sizeof(seq_t)));
#else
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
#endif
        if (!cache) abort();        /* allocation failure */
        cache_cap = new_cap;
    }

    /* If the requested value is already cached, return it immediately. */
    if (n <= cached_max)
        return cache[(size_t)n];

    /* Compute successive terms up to n, reusing previously computed
       values in the cache.  All intermediate arithmetic uses the type
       seq_t; conversions to idx_t are performed only when indexing the
       cache array. */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        if (i <= 9) {                       /* initial values */
            cache[(size_t)i] = (seq_t)(10 - (idx_t)i);
        } else {                            /* recurrence */
            idx_t t1 = i - (idx_t)cache[(size_t)(i - 3)];
            idx_t t2 = i - (idx_t)cache[(size_t)(i - 6)];
            cache[(size_t)i] = cache[(size_t)t1] + cache[(size_t)t2];
        }
    }
    cached_max = n;
    return cache[(size_t)n];
}
