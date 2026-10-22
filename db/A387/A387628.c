/*
 * A387628  a(n) = Sum_{k=0..floor(n/3)} 2^k * binomial(2*n-4*k+1,2*k+1).
 * https://oeis.org/A387628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 49, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A387628
   Formula (OEIS): a(n) = Sum_{k=0..⌊n/3⌋} 2^k * binomial(2*n-4*k+1, 2*k+1)
   Recurrence (given in OEIS): a(n) = 2*a(n-1) - a(n-2) + 4*a(n-3) + 4*a(n-4) - 4*a(n-6)
   Initial values: a(0)=1, a(1)=3, a(2)=5, a(3)=9, a(4)=29, a(5)=81, a(6)=185   */
#include <stdlib.h>

static const seq_t _base_vals[7] = {
    (seq_t)1,   /* a(0) */
    (seq_t)3,   /* a(1) */
    (seq_t)5,   /* a(2) */
    (seq_t)9,   /* a(3) */
    (seq_t)29,  /* a(4) */
    (seq_t)81,  /* a(5) */
    (seq_t)185  /* a(6) */
};

/* Static cache for memoised values.  Cache holds values up to max_computed. */
static seq_t * _cache = NULL;
static size_t  _cap    = 0;      /* allocated size of _cache */
static idx_t   _max_computed = (idx_t)-1;   /* last n that has been computed */

/* Grow the cache to be able to store values up to n.  The cache is
   grown exponentially to keep amortised O(1) cost for allocations. */
static void _ensure_cache(idx_t n)
{
    size_t required = (size_t)n + 1;
    if (_cache == NULL) {
        _cap = required > 8 ? required : 8;
        _cache = (seq_t *)malloc(_cap * sizeof(seq_t));
        for (size_t i = 0; i < 7; ++i)
            _cache[i] = _base_vals[i];
        _max_computed = 6;
    } else if (required > _cap) {
        size_t new_cap = _cap * 2;
        while (required > new_cap) new_cap *= 2;
        _cache = (seq_t *)realloc(_cache, new_cap * sizeof(seq_t));
        _cap = new_cap;
    }
}

/* Compute the value of a(n) and store it in the cache, if it has not
   been computed yet.  Calls are processed in arbitrary order. */
static void _compute_up_to(idx_t n)
{
    if (_max_computed >= n) return;   /* nothing to do */

    for (idx_t i = _max_computed + 1; i <= n; ++i) {
        /* Recurrence is valid for i >= 7; for i <=6 we already have the
           correct values from _base_vals. */
        if (i <= 6) {
            _cache[i] = _base_vals[i];
        } else {
            /* a(i) = 2*a(i-1) - a(i-2) + 4*a(i-3) + 4*a(i-4) - 4*a(i-6) */
            seq_t term = 2 * _cache[i-1]
                       - _cache[i-2]
                       + 4 * _cache[i-3]
                       + 4 * _cache[i-4]
                       - 4 * _cache[i-6];
            _cache[i] = term;
        }
    }
    _max_computed = n;
}

seq_t a(idx_t n)
{
    _ensure_cache(n);
    _compute_up_to(n);
    return _cache[n];
}
