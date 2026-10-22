/*
 * A303313  Number of nX7 0..1 arrays with every element equal to 0, 1, 4, 5
 *          or 6 horizontally, diagonally or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A303313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence
 *            a(n)=a(n-1)+a(n-2)+36a(n-3)+23a(n-4)+15a(n-5)-252a(n-6)
 *            -288a(n-7)-81a(n-8)+830a(n-9)+496a(n-10)+172a(n-11)
 *            -720a(n-12)-184a(n-13)-396a(n-14)+352a(n-15)-248a(n-16)
 *            +112a(n-17)-176a(n-18)-64a(n-20) for n>23. The first 23 terms
 *            are 21,23,158,439,1799,7096,26184,...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The recurrence above is empirical; the first 23 terms are used as seeds
   and are not part of any lookup table. */

#include <stdlib.h>
#include "../utils/mul_small.h"

/* Initial 23 terms of the sequence */
static const seq_t _init_vals[23] = {
    (seq_t)21,  (seq_t)23,  (seq_t)158,  (seq_t)439,  (seq_t)1799,
    (seq_t)7096, (seq_t)26184, (seq_t)100128, (seq_t)387892,
    (seq_t)1473950, (seq_t)5617780, (seq_t)21537368, (seq_t)82283698,
    (seq_t)314136752, (seq_t)1201134460, (seq_t)4591062616,
    (seq_t)17541004106, (seq_t)67038118694, (seq_t)256214474112,
    (seq_t)979116873266, (seq_t)3741800363318, (seq_t)14300090089956,
    (seq_t)54649561868376
};

/* Offsets and coefficients for the recurrence (n>23).  The term a(n-19)
   has coefficient 0, so it is omitted. */
static const int _offsets[19] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 20
};
static const int _coeffs[19] = {
    1,  1,  36, 23, 15, -252, -288, -81, 830, 496, 172,
    -720, -184, -396, 352, -248, 112, -176, -64
};

/* Cache for computed values: cache[i] holds a(i+1).  The cache is grown
   as needed; no external libraries are required. */
static seq_t * _cache = NULL;
static idx_t _cache_len = 0;   /* number of terms currently stored */
static idx_t _cache_cap = 0;   /* allocated capacity */

/* Ensure the cache has room for at least 'need' terms. */
static void
ensure_cache_capacity(idx_t need)
{
    if (_cache_cap >= need)
        return;
    idx_t new_cap = _cache_cap > 0 ? _cache_cap : 256;
    while (new_cap < need)
        new_cap <<= 1;
    _cache = (seq_t *)realloc(_cache, new_cap * sizeof(seq_t));
    _cache_cap = new_cap;
}

/* Compute the nth term (n≥1).  The function is safe for arbitrary
   calling order and uses memoisation. */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* safety: the sequence is defined for n≥1 */

    /* Grow the cache if necessary. */
    if (_cache_len < n) {
        ensure_cache_capacity(n);
        for (idx_t i = _cache_len + 1; i <= n; ++i) {
            seq_t val;
            if (i <= 23) {
                val = _init_vals[i - 1];
            } else {
                seq_t pos = 0, neg = 0;
                for (int k = 0; k < 19; ++k) {
                    idx_t idx = i - (idx_t)_offsets[k];   /* 1‑based index */
                    seq_t term = _cache[idx - 1];         /* already known */
                    int coeff = _coeffs[k];
                    if (coeff >= 0) {
                        pos = pos + mul_small(term, (unsigned int)coeff);
                    } else {
                        neg = neg + mul_small(term, (unsigned int)(-coeff));
                    }
                }
                /* Since the final value is non‑negative, pos ≥ neg. */
                val = pos - neg;
            }
            _cache[i - 1] = val;
        }
        _cache_len = n;
    }
    return _cache[n - 1];
}
