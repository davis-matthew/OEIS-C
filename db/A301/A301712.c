/*
 * A301712  Coordination sequence for node of type V1 in "usm" 2-D tiling (or
 *          net).
 * https://oeis.org/A301712
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) - a(n-4) + 2*a(n-5) - a(n-6) + a(n-9) - a(n-10)
 *            for n ≥ 11, with initial values a(0)=1, a(1)=5, a(2)=10,
 *            a(3)=16, a(4)=22, a(5)=27, a(6)=33, a(7)=38, a(8)=43, a(9)=49,
 *            a(10)=53
 * Range:     u8 n <= 47, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* for malloc, realloc, free */

static const seq_t _a_init[11] = {
    (seq_t)1, (seq_t)5, (seq_t)10, (seq_t)16, (seq_t)22,
    (seq_t)27, (seq_t)33, (seq_t)38, (seq_t)43, (seq_t)49, (seq_t)53
};

static seq_t * _a_cache   = NULL;
static size_t  _a_len     = 0;   /* number of computed entries */
static size_t  _a_cap     = 0;   /* allocated capacity */

/* Ensure that cache has entries up to index n (inclusive). */
static void _a_ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;

    if (need <= _a_len)
        return;

    /* --- allocation / reallocation ----------------------------------- */
    if (_a_cache == NULL) {
        /* first allocation: keep at least 'need' slots and a free margin */
        _a_cap = need * 2;
        if (_a_cap < need)               /* guard against overflow */
            _a_cap = need;
        _a_cache = (seq_t *)malloc(_a_cap * sizeof(seq_t));
        if (_a_cache == NULL)
            exit(1);                      /* allocation failed */

        /* initialise the first eleven values */
        for (size_t i = 0; i < 11 && i < need; ++i)
            _a_cache[i] = _a_init[i];
        _a_len = 11;
    } else if (need > _a_cap) {
        _a_cap = need * 2;
        if (_a_cap < need)               /* guard against overflow */
            _a_cap = need;
        _a_cache = (seq_t *)realloc(_a_cache, _a_cap * sizeof(seq_t));
        if (_a_cache == NULL)
            exit(1);                      /* reallocation failed */
    }

    /* --- compute further terms ------------------------------------- */
    for (size_t i = _a_len; i < need; ++i) {
        /* recurrence: a_i = a_{i-1} - a_{i-4} + 2*a_{i-5} - a_{i-6}
                        + a_{i-9} - a_{i-10} */
        seq_t t = _a_cache[i-1] - _a_cache[i-4];
        t += _a_cache[i-5] + _a_cache[i-5];
        t -= _a_cache[i-6];
        t += _a_cache[i-9];
        t -= _a_cache[i-10];
        _a_cache[i] = t;
    }

    _a_len = need;
}

seq_t a(idx_t n)
{
    if (n < 11)
        return _a_init[(size_t)n];

    _a_ensure_cache(n);
    return _a_cache[(size_t)n];
}
