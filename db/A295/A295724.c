/*
 * A295724  a(n) = a(n-1) + 3*a(n-2) -2*a(n-3) - 2*a(n-4), where a(0) = 0,
 *          a(1) = 0, a(2) = 1, a(3) = 2.
 * https://oeis.org/A295724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - 2*a(n-3) - 2*a(n-4) * with a(0)=0,
 *            a(1)=0, a(2)=1, a(3)=2
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* array of computed terms */
static size_t cache_len = 0;         /* number of terms stored */
static size_t cache_cap = 0;         /* allocated capacity */

/* grow the cache to hold at least 'needed' elements (0‑based) */
static void ensure_capacity(size_t needed)
{
    if (needed < cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 4;
    while (new_cap <= needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* initialise base values a(0)..a(3) */
static void init_base(void)
{
    cache[0] = (seq_t)0;
    cache[1] = (seq_t)0;
    cache[2] = (seq_t)1;
    cache[3] = (seq_t)2;
    cache_len = 4;
}

seq_t a(idx_t n)
{
    /* handle the trivial cases directly */
    if (n < (idx_t)4) {
        switch ((long long)n) {
        case 0: return (seq_t)0;
        case 1: return (seq_t)0;
        case 2: return (seq_t)1;
        case 3: return (seq_t)2;
        default: return (seq_t)0; /* unreachable */
        }
    }

    /* lazily allocate cache if needed */
    if (!cache) {
        ensure_capacity(3);    /* capacity for indices 0..3 */
        init_base();
    }

    /* ensure we have space for the requested index */
    if ((size_t)n + 1 > cache_cap)
        ensure_capacity((size_t)n);

    /* compute any missing terms up to n */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        size_t idx = (size_t)i;
        /* recurrence: a(i) = a(i-1) + 3*a(i-2) - 2*a(i-3) - 2*a(i-4) */
        cache[idx] = cache[idx-1] + 3 * cache[idx-2]
                     - 2 * cache[idx-3] - 2 * cache[idx-4];
    }
    cache_len = (size_t)n + 1; /* we now have terms up to n */

    return cache[(size_t)n];
}
