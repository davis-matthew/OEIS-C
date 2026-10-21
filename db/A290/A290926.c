/*
 * A290926  p-INVERT of the positive integers, where p(S) = (1 - S^2)^2.
 * https://oeis.org/A290926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 26*a(n-2) + 48*a(n-3) - 59*a(n-4) + 48*a(n-5)
 *            - 26*a(n-6) + 8*a(n-7) - a(n-8) Initial values: a(0)=0, a(1)=2,
 *            a(2)=8, a(3)=23, a(4)=64, a(5)=182, a(6)=520, a(7)=1475
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 44, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoized sequence values          */
static size_t cache_cap = 0;     /* capacity in terms (next free)     */

static void ensure_cache(idx_t n)
{
    if (n < cache_cap)
        return;      /* already computed */

    size_t new_cap = (size_t)n + 1;
    if (new_cap < 8)
        new_cap = 8;            /* at least 8 terms for recurrence */

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();                /* allocation failed */

    /* initialise first 8 terms if the cache was empty */
    if (cache == NULL) {
        new_cache[0] = 0;
        new_cache[1] = 2;
        new_cache[2] = 8;
        new_cache[3] = 23;
        new_cache[4] = 64;
        new_cache[5] = 182;
        new_cache[6] = 520;
        new_cache[7] = 1475;
    }

    /* fill missing terms up to new_cap-1 */
    size_t start = cache_cap;
    if (start < 8)
        start = 8;              /* first 8 already present */

    for (size_t i = start; i < new_cap; ++i) {
        new_cache[i] = 8*new_cache[i-1]
                     - 26*new_cache[i-2]
                     + 48*new_cache[i-3]
                     - 59*new_cache[i-4]
                     + 48*new_cache[i-5]
                     - 26*new_cache[i-6]
                     + 8*new_cache[i-7]
                     - new_cache[i-8];
    }

    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
