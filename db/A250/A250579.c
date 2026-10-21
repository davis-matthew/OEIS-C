/*
 * A250579  Number of (n+1) X (4+1) 0..1 arrays with nondecreasing
 *          max(x(i,j),x(i,j-1)) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 10*a(n-2) - 7*a(n-3) + 40*a(n-4) - 40*a(n-5) *
 *            - 2*a(n-6) + 29*a(n-7) - 19*a(n-8) + 4*a(n-9) * Initial values:
 *            a(1)=125, a(2)=329, a(3)=778, a(4)=1776, a(5)=4013, * a(6)=9055,
 *            a(7)=20510, a(8)=46664, a(9)=106695, a(10)=244907
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 21, u64 n <= 47, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static const seq_t _init[10] = {
    (seq_t)125, (seq_t)329, (seq_t)778, (seq_t)1776, (seq_t)4013,
    (seq_t)9055, (seq_t)20510, (seq_t)46664, (seq_t)106695, (seq_t)244907
};

static seq_t * _cache = NULL;   /* 1‑based array of computed terms */
static idx_t   _comp_max = 0;   /* maximum n that has been computed */
static size_t  _cache_cap = 0;  /* current capacity (size‑1 for 1‑based) */

/* fetch term k, using cache for k>10 */
static inline seq_t _term(idx_t k)
{
    return (k <= 10) ? _init[k - 1] : _cache[k];
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n <= 10) {
        return _init[n - 1];
    }

    /* initialise cache on first call */
    if (_cache == NULL) {
        _cache_cap = 16;                                 /* initial capacity */
        _cache = (seq_t*)calloc(_cache_cap + 1, sizeof(seq_t));
        if (_cache == NULL) { return (seq_t)0; }
        for (idx_t i = 1; i <= 10; ++i) _cache[i] = _init[i - 1];
        _comp_max = 10;
    }

    /* increase capacity if needed */
    while (_cache_cap < n) {
        size_t new_cap = (_cache_cap ? _cache_cap * 2 : 16);
        seq_t *tmp = (seq_t*)realloc(_cache, (new_cap + 1) * sizeof(seq_t));
        if (tmp == NULL) { return (seq_t)0; }
        _cache = tmp;
        _cache_cap = new_cap;
    }

    /* compute new terms up to n */
    for (idx_t i = _comp_max + 1; i <= n; ++i) {
        seq_t a1 = _term(i - 1);
        seq_t a2 = _term(i - 2);
        seq_t a3 = _term(i - 3);
        seq_t a4 = _term(i - 4);
        seq_t a5 = _term(i - 5);
        seq_t a6 = _term(i - 6);
        seq_t a7 = _term(i - 7);
        seq_t a8 = _term(i - 8);
        seq_t a9 = _term(i - 9);

        seq_t pos = (seq_t)6 * a1 + (seq_t)40 * a4 + (seq_t)29 * a7 + (seq_t)4 * a9;
        seq_t neg = (seq_t)10 * a2 + (seq_t)7 * a3 + (seq_t)40 * a5 + (seq_t)2 * a6 + (seq_t)19 * a8;

        _cache[i] = pos - neg;   /* all terms are non‑negative */
    }

    _comp_max = n;
    return _cache[n];
}
