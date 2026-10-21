/*
 * A287827  Number of sequences over the alphabet {0,1,...,9} such that no two
 *          consecutive terms have distance 3.
 * https://oeis.org/A287827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - a(n-2) - 20*a(n-3) + 10*a(n-4); a(0)=1,
 *            a(1)=10, a(2)=86, a(3)=742, a(4)=6404
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;
static idx_t computed_max = (idx_t)-1;

seq_t a(idx_t n)
{
    /* handle the first five terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;
    if (n == 2) return (seq_t)86;
    if (n == 3) return (seq_t)742;
    if (n == 4) return (seq_t)6404;

    /* ensure cache is allocated and initialised */
    if (!cache) {
        cache_size = 8; /* initial capacity */
        cache = (seq_t *)realloc(NULL, cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)10;
        cache[2] = (seq_t)86;
        cache[3] = (seq_t)742;
        cache[4] = (seq_t)6404;
        computed_max = 4;
    }

    /* grow the cache if needed */
    if (cache_size <= n) {
        size_t new_size = cache_size;
        while (new_size <= n) new_size <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failed */
        cache = tmp;
        cache_size = new_size;
    }

    /* compute any missing terms up to n */
    if (computed_max < n) {
        for (idx_t i = computed_max + 1; i <= n; ++i) {
            seq_t t = (seq_t)9 * cache[(size_t)(i - 1)];
            t -= cache[(size_t)(i - 2)];
            t -= (seq_t)20 * cache[(size_t)(i - 3)];
            t += (seq_t)10 * cache[(size_t)(i - 4)];
            cache[(size_t)i] = t;
        }
        computed_max = n;
    }

    return cache[(size_t)n];
}
