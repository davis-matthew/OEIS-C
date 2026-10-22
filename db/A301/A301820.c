/*
 * A301820  Number of nX5 0..1 arrays with every element equal to 1 or 2
 *          horizontally or antidiagonally adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 22*a(n-2) + 28*a(n-3) - 17*a(n-4) + 4*a(n-5),
 *            for n>7 * Initial values: a(1)=3, a(2)=18, a(3)=74, a(4)=299,
 *            a(5)=1197, a(6)=4785, a(7)=19130 * * This implementation uses
 *            simple memoisation. For C++ builds it employs *
 *            std::vector<seq_t>; for C builds it uses malloc/realloc. The...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <vector>
#endif

static idx_t cache_max = 0;  /* highest n for which a(n) has been computed */

#ifdef __cplusplus
static std::vector<seq_t> cache;   /* cache[0] is unused, indices start at 1 */
#else
static seq_t *cache = NULL;        /* cache[0] is unused */
#endif

/* initialise base values a(1)..a(7) if they are not yet present */
static void init_base(void)
{
#ifdef __cplusplus
    if (cache.size() < 8) {
        cache.resize(8);  /* indices 0..7 */
    }
    cache[1] = (seq_t)3;
    cache[2] = (seq_t)18;
    cache[3] = (seq_t)74;
    cache[4] = (seq_t)299;
    cache[5] = (seq_t)1197;
    cache[6] = (seq_t)4785;
    cache[7] = (seq_t)19130;
#else
    if (cache == NULL || cache_max < 7) {
        size_t newsize = 8;
        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cache_max = 0;
    }
    cache[1] = (seq_t)3;
    cache[2] = (seq_t)18;
    cache[3] = (seq_t)74;
    cache[4] = (seq_t)299;
    cache[5] = (seq_t)1197;
    cache[6] = (seq_t)4785;
    cache[7] = (seq_t)19130;
#endif
}

/* ensure that a(1) .. a(n) are computed and stored */
static void ensure_cache(idx_t n)
{
    if (n <= cache_max) return;

    init_base();          /* guarantee base values are present */

#ifdef __cplusplus
    if (cache.size() <= n) {
        cache.resize(n + 1);   /* indices [0..n] */
    }
#else
    if (cache == NULL) {
        cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (cache == NULL) abort();
        cache_max = 0;
    } else if (cache_max < n) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
    }
#endif

    /* compute missing terms */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i <= 7) continue;  /* already initialised by init_base() */
        seq_t val = (seq_t)8  * cache[i-1]
                  - (seq_t)22 * cache[i-2]
                  + (seq_t)28 * cache[i-3]
                  - (seq_t)17 * cache[i-4]
                  + (seq_t)4  * cache[i-5];
        cache[i] = val;
    }
    cache_max = n > cache_max ? n : cache_max;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    ensure_cache(n);
    return cache[n];
}
