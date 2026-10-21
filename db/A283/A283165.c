/*
 * A283165  a(0) = 0; a(1) = 1; a(2*n) = 2*a(n), a(2*n+1) = 2*a(n) +
 *          (-1)^a(n+1).
 * https://oeis.org/A283165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1, a(2*n)=2*a(n), a(2*n+1)=2*a(n)+(-1)^a(n+1)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoisation */
static seq_t * _cache = NULL;
static size_t  _cap   = 0;     /* allocated size of _cache */
static size_t  _next  = 0;     /* next index to compute */

/* grow the cache to at least new_cap entries */
static void _grow_cache(size_t new_cap)
{
    seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!tmp) abort();                 /* allocation failure */
    /* copy existing values */
    for (size_t i = 0; i < _next; ++i)
        tmp[i] = _cache[i];
    /* free old storage */
    free(_cache);
    _cache = tmp;
    _cap   = new_cap;
}

/* compute a(n) with memoisation, works for any order of calls */
seq_t a(idx_t n)
{
    size_t nn = (size_t)n;        /* idx_t is uint64_t, fits in size_t for n ≤ 10000 */
    if (nn >= _cap) {             /* grow if necessary */
        size_t new_cap = _cap ? _cap * 2 : 1;
        while (new_cap <= nn) new_cap <<= 1;
        _grow_cache(new_cap);
    }
    for (size_t i = _next; i <= nn; ++i) {   /* compute missing entries */
        if (i == 0) {
            _cache[i] = 0;
        } else if (i == 1) {
            _cache[i] = 1;
        } else if ((i & 1) == 0) {           /* even index: i = 2*n */
            _cache[i] = 2 * _cache[i / 2];
        } else {                            /* odd index: i = 2*n+1 */
            seq_t ak  = _cache[(i - 1) / 2];
            seq_t ak1 = _cache[(i - 1) / 2 + 1];
            seq_t base = 2 * ak;
            /* (-1)^a(n+1): +1 if a(n+1) even, -1 if odd */
            _cache[i] = (ak1 & 1) ? base - 1 : base + 1;
        }
    }
    _next = nn + 1;
    return _cache[nn];
}
