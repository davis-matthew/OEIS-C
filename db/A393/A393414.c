/*
 * A393414  Fibonacci sequence beginning 29, 21.
 * https://oeis.org/A393414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A393414: a(n)=a(n-1)+a(n-2) with a(0)=29, a(1)=21 */

#include <stdlib.h>

static seq_t * _cache = NULL;     /* cache of already computed values */
static size_t _cache_cap = 0;     /* current allocated size (in seq_t slots) */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly */
    if (n < 2) {
        return n == 0 ? (seq_t)29 : (seq_t)21;
    }

    /* Ensure cache has capacity for indices 0 and 1 */
    if (_cache_cap < 2) {
        _cache_cap = 2;
        _cache = (seq_t *)realloc(_cache, _cache_cap * sizeof(seq_t));
        _cache[0] = 29;
        _cache[1] = 21;
    }

    /* If requested index exceeds cache, extend and compute new terms */
    if ((size_t)n >= _cache_cap) {
        size_t new_cap = (size_t)n + 1;
        _cache = (seq_t *)realloc(_cache, new_cap * sizeof(seq_t));

        for (size_t i = _cache_cap; i <= (size_t)n; ++i) {
            _cache[i] = _cache[i - 1] + _cache[i - 2];
        }
        _cache_cap = new_cap;
    }

    return _cache[(size_t)n];
}
