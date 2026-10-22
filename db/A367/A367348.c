/*
 * A367348  a(1) = 1, a(2) = 11; thereafter, a(n+1) = a(n) + x + 10*y, where x
 *          and y are respectively the first and last digits of a(n).
 * https://oeis.org/A367348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 1391, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1) = 1, a(2) = 11;
   for n >= 2:  a(n+1) = a(n) + first_digit(a(n)) + 10 * last_digit(a(n)). */

#include <stdlib.h>

static idx_t _a_cache_nmax = 0;
static seq_t * _a_cache = NULL;
static size_t   _a_cache_cap = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* Undefined, but safe fallback */

    /* Allocate or grow the cache to hold index n */
    if (_a_cache == NULL) {
        _a_cache_cap = 8;                     /* arbitrary small start */
        _a_cache = (seq_t *)malloc(_a_cache_cap * sizeof(seq_t));
        if (!_a_cache) abort();
    }
    if (_a_cache_cap <= n) {
        size_t new_cap = (size_t)(n + 2);      /* leave space for index 0 unused */
        _a_cache = (seq_t *)realloc(_a_cache, new_cap * sizeof(seq_t));
        if (!_a_cache) abort();
        _a_cache_cap = new_cap;
    }

    /* Ensure the first two terms are defined */
    if (_a_cache[1] == 0) _a_cache[1] = 1;
    if (n >= 2 && _a_cache[2] == 0) _a_cache[2] = 11;

    /* If already computed up to n, just return it */
    if (n <= _a_cache_nmax) return _a_cache[(size_t)n];

    /* Compute missing terms */
    idx_t start = _a_cache_nmax + 1;
    if (start < 3) start = 3;                   /* base terms already known */

    for (idx_t i = start; i <= n; ++i) {
        seq_t prev = _a_cache[(size_t)(i - 1)];
        seq_t last = prev % (seq_t)10;

        seq_t first = prev;
        while (first >= (seq_t)10)
            first /= (seq_t)10;

        _a_cache[(size_t)i] = prev + first + (seq_t)10 * last;
    }

    _a_cache_nmax = n;
    return _a_cache[(size_t)n];
}
