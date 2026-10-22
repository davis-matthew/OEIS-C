/*
 * A373718  Expansion of 1/(1 - x * (1 + x^2)^3).
 * https://oeis.org/A373718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 66, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A373718
 * Formula: a(n) = a(n-1) + 3*a(n-3) + 3*a(n-5) + a(n-7)
 * Initial values: a(0)=1, a(1)=1, a(2)=1, a(3)=4, a(4)=7, a(5)=13, a(6)=28
 */

#include <stddef.h>
#include <stdlib.h>

/* Internal cache for the sequence. */
static seq_t * _cache     = NULL;   /* array of computed terms          */
static size_t _cache_cap  = 0;      /* allocated capacity (in elements)*/
static idx_t  _highest    = 0;      /* highest index that has been computed */

/* Grow the cache array to hold indices up to n (inclusive). */
static void _ensure_cache(idx_t n)
{
    if (_cache_cap > n) return;

    /* Allocate at least n+1 elements, doubling strategy for speed. */
    size_t new_cap = (size_t)(n) + 1;
    if (new_cap < 8) new_cap = 8; /* initial small size */
    if (new_cap < _cache_cap * 2) new_cap = _cache_cap * 2;

    seq_t *tmp = (seq_t *)realloc(_cache, new_cap * sizeof(seq_t));
    /* If realloc fails, abort – the caller will not get a result anyway. */
    if (!tmp) abort();
    _cache = tmp;
    _cache_cap = new_cap;
}

/* Initialise the cache with the known initial terms. */
static void _init_cache(void)
{
    if (_cache != NULL) return; /* already initialised */

    _ensure_cache(6);           /* at least indices 0..6 */

    _cache[0] = (seq_t)1;
    _cache[1] = (seq_t)1;
    _cache[2] = (seq_t)1;
    _cache[3] = (seq_t)4;
    _cache[4] = (seq_t)7;
    _cache[5] = (seq_t)13;
    _cache[6] = (seq_t)28;

    _highest = 6;
}

/* Compute a(n) using memoised recurrence. */
seq_t a(idx_t n)
{
    _init_cache();

    if (n <= _highest) return _cache[n];

    /* Extend cache and compute values up to n. */
    _ensure_cache(n);
    for (idx_t i = _highest + 1; i <= n; ++i) {
        /* For i >= 7 recurrence uses indices i-7 etc.;
           for i < 7 the base values already exist.        */
        seq_t val = _cache[i - 1];
        val += (seq_t)3 * _cache[i - 3];
        val += (seq_t)3 * _cache[i - 5];
        val += _cache[i - 7];
        _cache[i] = val;
    }
    _highest = n;
    return _cache[n];
}
