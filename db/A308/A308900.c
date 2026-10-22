/*
 * A308900  An explicit example of an infinite sequence with a(1)=1 and, for n
 *          >= 2, a(n) and S(n) = Sum_{i=1..n} a(i) have no digit in common.
 * https://oeis.org/A308900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = -a(n-1) + 10*a(n-2) + 10*a(n-3) with a(1)=1, a(2)=6,
 *            a(3)=4. The terms are computed iteratively and cached so
 *            subsequent calls are O(1) after the first growth to the
 *            requested index
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;    /* cache[0] holds a(1) */
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t cache_len = 0;   /* number of terms currently stored */

    if (n == 0) return (seq_t)0;   /* defined only for n>=1 */

    /* initialise base values a(1)=1, a(2)=6, a(3)=4 */
    if (cache_len < 3) {
        if (cache_cap < 3) {
            cache_cap = 3;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            if (!cache) abort();
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)4;
        cache_len = 3;
    }

    /* return cached value if already computed */
    if ((size_t)n <= cache_len) return cache[n - 1];

    /* grow cache to hold up to n */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* i-1 = current index in cache (0‑based) */
        seq_t val = -cache[i - 2]      /* -a(i-1) */
                  + 10 * cache[i - 3]  /* +10*a(i-2) */
                  + 10 * cache[i - 4]; /* +10*a(i-3) */
        cache[i - 1] = val;
    }
    cache_len = (size_t)n;
    return cache[n - 1];
}
