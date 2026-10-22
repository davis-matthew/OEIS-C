/*
 * A331124  Number of function evaluations in a recursive calculation of
 *          Fibonacci(n).
 * https://oeis.org/A331124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = 1 for n = 0, 1, 2 a(n) = a((n+1)/2) + a((n-1)/2) + 1 for
 *            odd n > 2 a(n) = a(n/2) + a(n/2-1) + a(n/2+1) + 1 for even n
 * Range:     u8 n <= 61, u16 n <= 3669, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Base cases */
    if (n < 3) return 1;

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static idx_t cache_top = 0;    /* highest index computed so far */

    /* Ensure cache large enough for n */
    if (!cache) {
        cache_cap = (size_t)n + 1;
        cache = (seq_t*)calloc(cache_cap, sizeof(seq_t));
        if (!cache) abort();               /* allocation failure */
        /* initialise known values */
        cache[0] = cache[1] = cache[2] = 1;
        cache_top = 2;
    } else if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();
        memset(new_cache + cache_cap, 0,
               (new_cap - cache_cap) * sizeof(seq_t));
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n */
    for (idx_t i = cache_top + 1; i <= n; ++i) {
        seq_t val;
        if (i & 1) {   /* odd */
            val = cache[(i + 1) / 2] + cache[(i - 1) / 2] + 1;
        } else {       /* even */
            val = cache[i / 2] + cache[i / 2 - 1] + cache[i / 2 + 1] + 1;
        }
        cache[i] = val;
    }
    cache_top = n;

    return cache[n];
}
