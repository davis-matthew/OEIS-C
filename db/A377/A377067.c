/*
 * A377067  Number of n X 3 0..2 matrices with row sums 3 and column sums n up
 *          to permutations of rows.
 * https://oeis.org/A377067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of xⁿ in (1 - x + x²)/((1 - x)⁵·(1 + x)²·(1 +
 *            x + x²)) Recurrence (for n ≥ 9): a(n) = 2 a(n‑1) + a(n‑2) – 3
 *            a(n‑3) – a(n‑4) + a(n‑5) + 3 a(n‑6) – a(n‑7) – 2 a(n‑8) + a(n‑9)
 *            (pₙ = 0 for n ≥ 3) Initial terms: a(0)…a(9) =
 *            1,1,4,6,12,18,30,42,63,85
 * Range:     u8 n <= 12, u16 n <= 62, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static const seq_t _init[10] = {
    1, 1, 4, 6, 12, 18, 30, 42, 63, 85
};

static seq_t * _cache    = NULL;
static size_t _cached_size = 0;
static size_t _cached_used = 0;

/* Ensure that _cache holds terms up to index n (inclusive). */
static void _ensure_cache(idx_t n)
{
    size_t idx = (size_t)n;
    if (idx < 10) return;                                   /* already known */

    size_t required = idx + 1;
    if (_cached_size < required) {
        size_t newsize = _cached_size > 0 ? _cached_size : 1;
        while (newsize < required) newsize <<= 1;
        void *tmp = realloc(_cache, newsize * sizeof(seq_t));
        if (!tmp) abort();
        _cache = (seq_t *)tmp;
        _cached_size = newsize;
    }

    if (_cached_used == 0) { /* initialise first 10 terms */
        for (size_t i = 0; i < 10; ++i) _cache[i] = _init[i];
        _cached_used = 10;
    }

    for (size_t i = _cached_used; i <= idx; ++i) {
        /* recurrence for i ≥ 10 (i.e. n ≥ 10) */
        _cache[i] = 2 * _cache[i-1]
                  + _cache[i-2]
                  - 3 * _cache[i-3]
                  - _cache[i-4]
                  + _cache[i-5]
                  + 3 * _cache[i-6]
                  - _cache[i-7]
                  - 2 * _cache[i-8]
                  + _cache[i-9];
    }
    _cached_used = idx + 1;
}

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;
    if (idx < 10) return _init[idx];
    _ensure_cache(n);
    return _cache[idx];
}
