/*
 * A301842  Number of 2Xn 0..1 arrays with every element equal to 0, 1 or 2
 *          horizontally or antidiagonally adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*********************************************************************
 * Sequence: A301842
 * Recurrence (for n > 6):
 *   a(n) = 3 * a(n-1) + a(n-2) - 2 * a(n-4)
 * Initial values:
 *   a(1) = 2,   a(2) = 8,   a(3) = 25,   a(4) = 81,
 *   a(5) = 263, a(6) = 855
 *********************************************************************/

#include <stdlib.h>

static const seq_t _base[6] = { (seq_t)2,  (seq_t)8,  (seq_t)25,
                                (seq_t)81, (seq_t)263, (seq_t)855 };

static seq_t * _cache = NULL;   /* dynamic cache of computed values   */
static size_t  _cache_len = 0;  /* number of terms currently stored   */
static size_t  _cache_cap = 0;  /* allocated capacity (bytes/pointers) */

/* Ensure that _cache contains at least n values (for a(1)..a(n)) */
static void _ensure_size(idx_t n)
{
    if (n == 0) return;          /* should not happen, but be safe */

    /* If already enough, nothing to do */
    if ((size_t)n <= _cache_len) return;

    size_t need = (size_t)n;
    size_t new_cap = _cache_cap;

    /* We always keep at least 6 slots so that the recurrence can start */
    if (new_cap < 6) new_cap = 6;

    if (new_cap < need) new_cap = need;

    /* Allocate or grow the cache */
    if (_cache == NULL) {
        _cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    } else {
        _cache = (seq_t *)realloc(_cache, new_cap * sizeof(seq_t));
    }

    /* If this is the first allocation, copy the initial 6 terms */
    if (_cache_len < 6) {
        size_t copy_len = (_cache_len < 6) ? 6 : _cache_len;
        for (size_t i = 0; i < copy_len; ++i) _cache[i] = _base[i];
        _cache_len = copy_len;
    }

    /* Compute missing terms using the recurrence */
    for (size_t i = _cache_len; i < need; ++i) {
        /* i corresponds to a(i+1) */
        seq_t term = 3 * _cache[i - 1];               /* 3*a(n-1) */
        term += _cache[i - 2];                        /* + a(n-2) */
        term -= 2 * _cache[i - 4];                    /* - 2*a(n-4) */
        _cache[i] = term;
    }

    _cache_len = need;
    _cache_cap = new_cap;
}

/* Sequence function as required by the build. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n<1, guard */
    _ensure_size(n);
    return _cache[(size_t)n - 1];
}
