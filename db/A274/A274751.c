/*
 * A274751  Number of 4 X n 0..2 arrays with no element equal to any value at
 *          offset (-1,-2) (0,-1) or (-1,0) and new values introduced in order
 *          0..2.
 * https://oeis.org/A274751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 20*a(n-2) + 10*a(n-3) - 44*a(n-4) * -
 *            20*a(n-5) + 31*a(n-6) + 2*a(n-7) - 4*a(n-8), * with initial
 *            values: * a(1)=4, a(2)=27, a(3)=144, a(4)=813, * a(5)=4554,
 *            a(6)=25585, a(7)=143634, a(8)=806695. * * This implementation
 *            caches computed values and expands the cache * as needed. I...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated capacity of cache      */
static size_t cache_count = 0; /* number of terms currently stored */

static void init_cache(void)
{
    if (cache_cap != 0)
        return;

    cache_cap = 8;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    /* base values a(1)..a(8) */
    cache[0] = (seq_t)4;
    cache[1] = (seq_t)27;
    cache[2] = (seq_t)144;
    cache[3] = (seq_t)813;
    cache[4] = (seq_t)4554;
    cache[5] = (seq_t)25585;
    cache[6] = (seq_t)143634;
    cache[7] = (seq_t)806695;
    cache_count = 8;
}

static void extend_cache(size_t need)
{
    if (need <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap < need)
        new_cap <<= 1;

    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;      /* undefined; not used but harmless */

    init_cache();

    if ((size_t)n <= cache_count)
        return cache[n - 1];

    /* Expand cache to at least n terms */
    extend_cache((size_t)n);

    /* Compute missing terms using the recurrence */
    for (idx_t i = cache_count + 1; i <= n; ++i) {
        /* indices are zero based: cache[i-2] is a(i-1) */
        seq_t val = (seq_t)2 * cache[(size_t)(i - 2)] +
                    (seq_t)20 * cache[(size_t)(i - 3)] +
                    (seq_t)10 * cache[(size_t)(i - 4)] -
                    (seq_t)44 * cache[(size_t)(i - 5)] -
                    (seq_t)20 * cache[(size_t)(i - 6)] +
                    (seq_t)31 * cache[(size_t)(i - 7)] +
                    (seq_t)2 * cache[(size_t)(i - 8)] -
                    (seq_t)4 * cache[(size_t)(i - 9)];
        cache[(size_t)(i - 1)] = val;
    }

    cache_count = (size_t)n;
    return cache[n - 1];
}
