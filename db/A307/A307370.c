/*
 * A307370  Number of integer partitions of n with 2 distinct parts, none
 *          appearing more than twice.
 * https://oeis.org/A307370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -a(n-1) + a(n-3) + 2*a(n-4) + a(n-5) - a(n-7) - a(n-8),
 *            a(0)=0, a(1)=0, a(2)=0, a(3)=1, a(4)=2, a(5)=4, a(6)=4, a(7)=6,
 *            a(8)=7
 * Range:     u8 n <= 207, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Ensure the cache is large enough and initialized (for C++ cast). */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *size_ptr)
{
    size_t needed = (size_t)(n + 1);
    if (*size_ptr < needed) {
        size_t new_size = (*size_ptr == 0) ? 64 : *size_ptr * 2;
        if (new_size < needed) new_size = needed;
        seq_t *newp = (seq_t*)realloc(*cache_ptr, new_size * sizeof(seq_t));
        if (!newp) abort();
        *cache_ptr = newp;
        *size_ptr = new_size;
    }
}

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static int initialized = 0;
    /* initial values for n=0..8 */
    static const seq_t init_vals[9] = {0,0,0,1,2,4,4,6,7};

    if (!initialized) {
        ensure_cache(8, &cache, &cache_size);
        for (int i = 0; i <= 8; ++i) cache[i] = init_vals[i];
        initialized = 1;
    }

    if (n > 8) {
        ensure_cache(n, &cache, &cache_size);
        for (idx_t i = 9; i <= n; ++i) {
            seq_t v = cache[i - 3];
            v += (cache[i - 4] << 1);   /* 2 * a(i-4) */
            v += cache[i - 5];
            v -= cache[i - 7];
            v -= cache[i - 8];
            v -= cache[i - 1];
            cache[i] = v;
        }
    }

    return cache[n];
}
