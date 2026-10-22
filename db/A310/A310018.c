/*
 * A310018  Coordination sequence Gal.4.34.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-6) + a(n-7) – a(n-13) for n > 23 * Base values (n = 0
 *            … 23): * 1, 3, 5, 10, 14, 17, 17, 20, 27, 33, 36, 40, 39, 39,
 *            44, 55, 65, * 64, 60, 61, 65, 72, 83, 91
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t _base[24] = {
    1, 3, 5, 10, 14, 17, 17, 20, 27, 33,
    36, 40, 39, 39, 44, 55, 65, 64, 60, 61,
    65, 72, 83, 91
};

static seq_t *_cache = NULL;
static size_t _cache_size = 0;
static idx_t _computed_until = (idx_t)-1;

seq_t a(idx_t n)
{
    /* Handle small indices directly. */
    if (n < 24) return _base[n];

    /* Initialize cache on first use. */
    if (_computed_until == (idx_t)-1) {
        /* Ensure cache has at least n+1 slots. */
        size_t sz = (size_t)n + 1;
        if (sz < 24) sz = 24;
        _cache = (seq_t *)malloc(sz * sizeof(seq_t));
        if (!_cache) return 0;              /* allocation failure – unlikely */
        for (idx_t i = 0; i < 24; ++i) _cache[i] = _base[i];
        _cache_size = sz;
        _computed_until = 23;
    }

    /* Extend cache if necessary. */
    if (n >= _cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(_cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;                /* allocation failure – unlikely */
        _cache = tmp;
        _cache_size = new_size;
    }

    /* Compute new values only once. */
    if (n > _computed_until) {
        for (idx_t i = _computed_until + 1; i <= n; ++i) {
            /* Recurrence holds for i >= 24 */
            _cache[i] = _cache[i - 6] + _cache[i - 7] - _cache[i - 13];
        }
        _computed_until = n;
    }

    return _cache[n];
}
