/*
 * A309792  Expansion of (2 + 6*x + 3*x^2 +4*x^3 - 10*x^4)/(1 - x - 4*x^4 +
 *          4*x^5).
 * https://oeis.org/A309792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 4*a(n-4) – 4*a(n-5) for n ≥ 5 initial terms:
 *            a(0)=2, a(1)=8, a(2)=5, a(3)=9, a(4)=7
 * Range:     u8 n <= 12, u16 n <= 28, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache (0‑based index 0..cache_size‑1) */
    static seq_t *cache   = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_size = 0;

    /* enlarge cache to at least n+1 elements (at least 5 for the
       initial values) */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 5;
        if (new_cap < n + 1)
            new_cap = n + 1;
        if (new_cap < 5)
            new_cap = 5;

        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failed – abort */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* initialise the first five values once */
    if (cache_size == 0) {
        cache[0] = 2; /* a(0) */
        cache[1] = 8; /* a(1) */
        cache[2] = 5; /* a(2) */
        cache[3] = 9; /* a(3) */
        cache[4] = 7; /* a(4) */
        cache_size = 5;
    }

    /* compute missing terms up to n */
    if (n >= cache_size) {
        for (idx_t i = cache_size; i <= n; ++i) {
            /* recurrence: a(i) = a(i-1) + 4*a(i-4) – 4*a(i-5) */
            cache[i] = cache[i-1] + 4 * cache[i-4] - 4 * cache[i-5];
        }
        cache_size = n + 1;
    }

    return cache[n];
}
