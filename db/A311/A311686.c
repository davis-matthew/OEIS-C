/*
 * A311686  Coordination sequence Gal.5.182.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) - a(n-4) + a(n-5) + a(n-11) - a(n-12) + a(n-15) - a(n-16)  (for n > 16)
   Initial values:
   a(0)=1, a(1)=4, a(2)=8, a(3)=13, a(4)=16, a(5)=20, a(6)=25, a(7)=29,
   a(8)=33, a(9)=38, a(10)=42, a(11)=46, a(12)=49, a(13)=53, a(14)=58,
   a(15)=61, a(16)=66 */
#include <stdlib.h>
#include <stddef.h>

/* initial 17 terms */
static const seq_t _init[17] = {
    1, 4, 8, 13, 16, 20, 25, 29, 33, 38, 42, 46, 49, 53, 58, 61, 66
};

/* dynamic cache for computed terms */
static seq_t * _cache = NULL;
static size_t _cache_len = 0; /* number of terms stored */
static size_t _cache_cap = 0; /* allocated capacity */

/* ensure cache contains terms up to index n */
static void _ensure_cache(idx_t n)
{
    size_t target = (size_t)n + 1;
    if (_cache_len >= target)
        return;

    if (_cache_cap < target) {
        size_t new_cap = target > 17 ? target : 17;
        seq_t *tmp = (seq_t *)realloc(_cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();              /* memory allocation failed */
        _cache = tmp;
        _cache_cap = new_cap;
    }

    for (size_t i = _cache_len; i < target; ++i) {
        if (i < 17)
            _cache[i] = _init[(size_t)i];
        else
            _cache[i] = _cache[i-1]            /* a(n-1)  */
                     - _cache[i-4]            /* -a(n-4) */
                     + _cache[i-5]            /* +a(n-5) */
                     + _cache[i-11]           /* +a(n-11) */
                     - _cache[i-12]           /* -a(n-12) */
                     + _cache[i-15]           /* +a(n-15) */
                     - _cache[i-16];          /* -a(n-16) */
    }
    _cache_len = target;
}

/* public entry point */
seq_t a(idx_t n)
{
    _ensure_cache(n);
    return _cache[(size_t)n];
}
