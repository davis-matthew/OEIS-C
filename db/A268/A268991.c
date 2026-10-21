/*
 * A268991  Number of n X 4 binary arrays with some element plus some
 *          horizontally or antidiagonally adjacent neighbor totalling two not
 *          more than once.
 * https://oeis.org/A268991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            16*a(n-1)-88*a(n-2)+200*a(n-3)-208*a(n-4)+96*a(n-5)-16*a(n-6)
 *            Initial values: a(1)=13, a(2)=103, a(3)=805, a(4)=6009,
 *            a(5)=43512, a(6)=308112, a(7)=2144780
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* initial values for n = 1..7 (index 0 unused) */
static const seq_t _base[8] = {
    0ULL,          /* dummy */
    13ULL,         /* a(1) */
    103ULL,        /* a(2) */
    805ULL,        /* a(3) */
    6009ULL,       /* a(4) */
    43512ULL,      /* a(5) */
    308112ULL,     /* a(6) */
    2144780ULL     /* a(7) */
};

static seq_t * _cache = NULL;
static size_t _cache_cap = 0;
static size_t _last_computed = 0;  /* highest index for which _cache[i] is valid */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1, undefined for 0 */

    /* ensure cache large enough for indices 0..n */
    size_t target = (size_t)n + 1;  /* +1 because index 0 is unused */
    if (_cache_cap < target) {
        _cache_cap = target;
        _cache = (seq_t *)realloc(_cache, _cache_cap * sizeof(seq_t));
    }

    /* compute missing values up to n */
    for (size_t i = _last_computed + 1; i <= (size_t)n; ++i) {
        if (i <= 7) {
            _cache[i] = _base[i];
        } else {
            /* sum of positive terms */
            seq_t pos = _cache[i-1] * 16 + _cache[i-3] * 200 + _cache[i-5] * 96;
            /* sum of negative terms */
            seq_t neg = _cache[i-2] * 88 + _cache[i-4] * 208 + _cache[i-6] * 16;
            _cache[i] = pos - neg;
        }
    }
    _last_computed = (size_t)n;
    return _cache[(size_t)n];
}
