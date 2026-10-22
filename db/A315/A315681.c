/*
 * A315681  Coordination sequence Gal.6.159.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4) - a(n-6) + a(n-7) + a(n-9) -
 *            a(n-10) for n > 10. * Initial values: * a(0)=1, a(1)=6, a(2)=12,
 *            a(3)=17, a(4)=22, a(5)=27, * a(6)=31, a(7)=35, a(8)=40, a(9)=45,
 *            a(10)=50
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Initial terms up to n = 10.  They fit in uint64_t; larger seq_t types truncate or cast safely. */
static const seq_t _init_vals[11] =
{ 1ULL, 6ULL, 12ULL, 17ULL, 22ULL, 27ULL, 31ULL, 35ULL, 40ULL, 45ULL, 50ULL };

/* Static cache for computed terms. */
static seq_t * _cache = NULL;     /* pointer to dense array of seq_t values */
static size_t _cache_size = 0;    /* allocated array size (number of elements) */
static size_t _computed   = 0;    /* number of elements actually filled (first index is 0) */

seq_t a(idx_t n)
{
    /* Direct lookup for the known initial segment. */
    if (n < 11)
        return _init_vals[n];

    /* First time we need terms beyond the initial segment: allocate and seed the cache. */
    if (_computed == 0) {
        size_t needed = (size_t)n + 1;
        if (needed < 12)                          /* at least accommodate a(10) */
            needed = 12;
        _cache = (seq_t*)malloc(needed * sizeof(seq_t));
        if (!_cache)
            exit(EXIT_FAILURE);
        for (size_t i = 0; i < 11; ++i)
            _cache[i] = _init_vals[i];
        _computed = 11;         /* indices 0..10 are known */
        _cache_size = needed;
    }

    /* Guarantee the cache can hold index n. */
    size_t target = (size_t)n;
    if (_cache_size <= target) {
        size_t new_size = _cache_size ? _cache_size * 2 : 16;
        if (new_size <= target)
            new_size = target + 1;
        seq_t *tmp = (seq_t*)realloc(_cache, new_size * sizeof(seq_t));
        if (!tmp)
            exit(EXIT_FAILURE);
        _cache = tmp;
        _cache_size = new_size;
    }

    /* Compute any missing terms up to n using the recurrence. */
    if (_computed <= target) {
        for (size_t i = _computed; i <= target; ++i) {
            /* The recurrence uses terms i-1, i-3, i-4, i-6, i-7, i-9, i-10. */
            seq_t val =
                _cache[i-1]  + _cache[i-3]
              - _cache[i-4] - _cache[i-6]
              + _cache[i-7] + _cache[i-9]
              - _cache[i-10];
            _cache[i] = val;
        }
        _computed = target + 1;
    }

    return _cache[target];
}
