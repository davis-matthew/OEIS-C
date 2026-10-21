/*
 * A251746  9-step Fibonacci sequence starting with 0,0,0,0,0,0,0,1,0.
 * https://oeis.org/A251746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 25, u32 n <= 41, u64 n <= 73, u128 n <= 137,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n+9) = a(n)+a(n+1)+…+a(n+8),
   initial terms: a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=0,
   a(5)=0, a(6)=0, a(7)=1, a(8)=0 */
#include <stddef.h>
#include <stdlib.h>

/* internal cache for computed terms */
static const seq_t _init[9] = {
    (seq_t)0,(seq_t)0,(seq_t)0,(seq_t)0,(seq_t)0,
    (seq_t)0,(seq_t)0,(seq_t)1,(seq_t)0
};

static seq_t * _cache        = NULL;   /* pointer to cached values */
static idx_t   _cache_size   = 0;      /* number of terms already stored */
static size_t  _cache_cap    = 0;      /* allocated capacity of _cache */
static seq_t   _sum_last9    = 0;      /* sum of the last 9 stored terms */

/* public function */
seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (_cache == NULL) {
        _cache_cap = 9;
        _cache = (seq_t *)malloc(_cache_cap * sizeof(seq_t));
        if (_cache == NULL)  /* allocation failed */
            return (seq_t)0;

        for (idx_t i = 0; i < 9; ++i)
            _cache[i] = _init[i];
        _cache_size = 9;

        /* compute initial sum of the first 9 terms */
        _sum_last9 = _init[0] + _init[1] + _init[2] + _init[3] + _init[4] +
                     _init[5] + _init[6] + _init[7] + _init[8];
    }

    /* if the requested value is already cached, return it */
    if (n < _cache_size)
        return _cache[n];

    /* enlarge the cache if needed */
    if ((size_t)n + 1 > _cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(_cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)  /* allocation failed */
            return (seq_t)0;
        _cache = tmp;
        _cache_cap = new_cap;
    }

    /* compute all missing terms up to n */
    for (idx_t i = _cache_size; i <= n; ++i) {
        /* recurrence: a(i) = sum of previous 9 terms */
        seq_t val = _sum_last9;
        _cache[i] = val;

        /* update running sum for the next iteration */
        _sum_last9 = _sum_last9 + val - _cache[i - 9];
    }

    _cache_size = n + 1;
    return _cache[n];
}
