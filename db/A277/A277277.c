/*
 * A277277  Number of overpal-free binary words of length n.
 * https://oeis.org/A277277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + a(n-4) for n>9; a(0)=1, a(1)=2, a(2)=4, a(3)=6,
 *            a(4)=10, a(5)=14, a(6)=20, a(7)=28, a(8)=36, a(9)=44
 * Range:     u8 n <= 16, u16 n <= 39, u32 n <= 85, u64 n <= 177,
 *            u128 n <= 361, big any n
 * Helpers:   _max
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/_max.h"

/* Base values for indices 0..9. */
static const seq_t _BASE[10] = {
    (seq_t)1, (seq_t)2, (seq_t)4, (seq_t)6,
    (seq_t)10, (seq_t)14, (seq_t)20, (seq_t)28,
    (seq_t)36, (seq_t)44
};

/* Dynamic cache for computed values. */
static seq_t * _CACHE = NULL;
static size_t _CAP    = 0;   /* capacity of _CACHE */
static size_t _NEC    = 0;   /* number of entries that are valid */

seq_t a(idx_t n) {
    size_t idx = (size_t)n;

    /* initialise the cache on first call */
    if (_CACHE == NULL) {
        _CAP = 10;
        _CACHE = (seq_t*)malloc(_CAP * sizeof(seq_t));
        if (!_CACHE) return (seq_t)0;   /* allocation failed */
        for (size_t i = 0; i < 10; ++i)
            _CACHE[i] = _BASE[i];
        _NEC = 10;
    }

    /* expand cache if needed */
    if (idx >= _CAP) {
        size_t new_cap = _max(_CAP * 2, idx + 1);
        seq_t *tmp = (seq_t*)realloc(_CACHE, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* realloc failed */
        _CACHE = tmp;
        _CAP = new_cap;
    }

    /* compute missing values up to idx */
    if (idx >= _NEC) {
        size_t start = _NEC;
        if (start < 10) start = 10;   /* base values already present */
        for (size_t i = start; i <= idx; ++i) {
            _CACHE[i] = _CACHE[i-2] + _CACHE[i-4];
        }
        _NEC = idx + 1;
    }

    return _CACHE[idx];
}
