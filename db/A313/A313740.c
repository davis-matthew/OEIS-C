/*
 * A313740  Coordination sequence Gal.5.309.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) - a(n-5) + 2*a(n-6) * - a(n-7) -
 *            a(n-10) + 2*a(n-11) - a(n-12) * a(0)=1, a(1)=5, a(2)=10,
 *            a(3)=15, a(4)=20, a(5)=25, * a(6)=31, a(7)=37, a(8)=43, a(9)=49,
 *            a(10)=55, * a(11)=60, a(12)=65
 * Range:     u8 n <= 48, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[13] = {
    1ULL, 5ULL, 10ULL, 15ULL, 20ULL, 25ULL, 31ULL,
    37ULL, 43ULL, 49ULL, 55ULL, 60ULL, 65ULL
};

seq_t a(idx_t n)
{
    /* return early for the first 13 terms */
    if (n < 13)
        return init[n];

    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* ensure storage for at least n+1 terms */
    size_t needed = (size_t)n + 1;
    if (!cache) {
        cache_cap = needed > 16 ? needed : 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_cap < needed) {
        cache_cap = needed;
        cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* initialize the first 13 entries once */
    if (cache_len <= 13) {
        for (idx_t i = 0; i < 13; ++i)
            cache[i] = init[i];
        cache_len = 13;
    }

    /* compute missing terms up to n */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        seq_t val = 2 * cache[i - 1];
        val -= cache[i - 2];
        val -= cache[i - 5];
        val += 2 * cache[i - 6];
        val -= cache[i - 7];
        val -= cache[i - 10];
        val += 2 * cache[i - 11];
        val -= cache[i - 12];
        cache[i] = val;
    }

    cache_len = (size_t)n + 1;
    return cache[n];
}
