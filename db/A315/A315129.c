/*
 * A315129  Coordination sequence Gal.6.401.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-9) - a(n-18) for n > 21, with the initial terms
 *            a(0) = 1, a(1) = 6, a(2) = 10, a(3) = 12, a(4) = 18, a(5) = 22,
 *            a(6) = 30, a(7) = 30, a(8) = 38, a(9) = 44, a(10) = 50, a(11) =
 *            54, a(12) = 54, a(13) = 62, a(14) = 66, a(15) = 68, a(16) = 74,
 *            a(17) = 82, a(18) = 88, a(19) = 94, a(20) =...
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static const seq_t _A315129_init[22] = {
    (seq_t)1,  (seq_t)6,  (seq_t)10, (seq_t)12, (seq_t)18,
    (seq_t)22, (seq_t)30, (seq_t)30, (seq_t)38, (seq_t)44,
    (seq_t)50, (seq_t)54, (seq_t)54, (seq_t)62, (seq_t)66,
    (seq_t)68, (seq_t)74, (seq_t)82, (seq_t)88, (seq_t)94,
    (seq_t)98, (seq_t)92
};

static seq_t *_A315129_cache = NULL;
static idx_t  _A315129_cache_size = 0; /* number of valid entries in cache */
static idx_t  _A315129_cache_cap  = 0; /* allocated capacity */

/* Ensure the cache contains all values up to n. */
static void _A315129_ensure(idx_t n)
{
    if (_A315129_cache == NULL) {
        /* Initial allocation; at least hold the known initial values. */
        _A315129_cache_cap = n + 1;
        if (_A315129_cache_cap < 64) _A315129_cache_cap = 64;
        _A315129_cache = (seq_t *)malloc(_A315129_cache_cap * sizeof(seq_t));
        /* Copy known initial values. */
        memcpy(_A315129_cache, _A315129_init,
               sizeof(_A315129_init));
        _A315129_cache_size = 22;
    } else if (n >= _A315129_cache_cap) {
        idx_t new_cap = _A315129_cache_cap * 2;
        if (new_cap <= n) new_cap = n + 1;
        _A315129_cache = (seq_t *)realloc(_A315129_cache, new_cap * sizeof(seq_t));
        _A315129_cache_cap = new_cap;
    }

    /* Compute any new values. */
    for (idx_t i = _A315129_cache_size; i <= n; ++i) {
        if (i < 22) continue; /* initial values already present */
        _A315129_cache[i] = (seq_t)2 * _A315129_cache[i - 9] - _A315129_cache[i - 18];
    }
    if (_A315129_cache_size <= n)
        _A315129_cache_size = n + 1;
}

seq_t a(idx_t n)
{
    if (n < 22) return _A315129_init[n];
    _A315129_ensure(n);
    return _A315129_cache[n];
}
