/*
 * A264202  Number of (n+1) X (2+1) arrays of permutations of 0..n*3+2 with
 *          each element having index change +-(.,.) 0,0 0,2 or 1,0.
 * https://oeis.org/A264202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 18*a(n-2) + 5*a(n-3) - 18*a(n-4) + 3*a(n-5) +
 *            a(n-6) with a(1)=18, a(2)=96, a(3)=605, a(4)=3600, a(5)=21853,
 *            a(6)=131712
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
/* seq_t and idx_t are defined elsewhere by the build system */

static const seq_t _base_vals[7] = {
    0,            /* index 0 unused */
    18,           /* a(1) */
    96,           /* a(2) */
    605,          /* a(3) */
    3600,         /* a(4) */
    21853,        /* a(5) */
    131712        /* a(6) */
};

static seq_t * _cache = NULL;  /* 1‑based cache, indices 1..max_n */
static idx_t   _cache_len = 0; /* last computed index +1 */

/* Ensure the cache can hold indices up to n */
static void _ensure_cache(idx_t n)
{
    if (_cache_len > n) return;        /* already large enough */
    idx_t new_len = n + 1;              /* we need indices 0..n (0 unused) */
    seq_t *new_cache = (seq_t*)realloc(_cache, new_len * sizeof(seq_t));
    if (!new_cache) return;            /* allocation failure propagates as zero */

    /* If expanding from zero, set base values */
    if (_cache == NULL) {
        for (idx_t i = 1; i <= 6; ++i) new_cache[i] = _base_vals[i];
        for (idx_t i = 7; i <= n; ++i) ; /* will be filled below */
    } else {
        /* If we are extending an existing cache, just add new space */
        for (idx_t i = _cache_len; i <= n; ++i) ; /* will be filled below */
    }

    _cache = new_cache;
    _cache_len = new_len;

    /* Compute new terms using recurrence */
    for (idx_t i = 7; i <= n; ++i) {
        _cache[i] =  3*_cache[i-1]
                   +18*_cache[i-2]
                   + 5*_cache[i-3]
                   -18*_cache[i-4]
                   + 3*_cache[i-5]
                   + 1*_cache[i-6];
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, undefined for 0 */
    if (n <= 6) return _base_vals[n];

    _ensure_cache(n);
    return _cache[n];
}
