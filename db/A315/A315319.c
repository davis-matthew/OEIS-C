/*
 * A315319  Coordination sequence Gal.5.289.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - 2*a(n-4) + 2*a(n-5) -
 *            2*a(n-6) + 2*a(n-7) - 2*a(n-8) + 2*a(n-9) - a(n-10) for n >= 11.
 *            Initial values: a(0)=1, a(1)=6, a(2)=10, a(3)=16, a(4)=21,
 *            a(5)=26, a(6)=31, a(7)=36, a(8)=42, a(9)=46, a(10)=52
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t _init[11] = {
    1, 6, 10, 16, 21, 26, 31, 36, 42, 46, 52
};

static seq_t * _cache = NULL;    /* dynamic array of computed terms */
static size_t   _cache_max = 0;   /* largest index currently stored */

seq_t a(idx_t n)
{
    /* Handle indices below the offset with the known values. */
    if (n < 11) return _init[(size_t)n];

    /* Prepare the cache if necessary. */
    if (_cache == NULL) {
        size_t cap = (size_t)n + 1;
        _cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (_cache == NULL) return (seq_t)0; /* out of memory */
        for (size_t i = 0; i < 11; ++i) _cache[i] = _init[i];
        _cache_max = 10;
    } else if (_cache_max < (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 2 * _cache_max) new_cap = 2 * _cache_max;
        seq_t *tmp = (seq_t *)realloc(_cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* out of memory */
        _cache = tmp;
    }

    if ((size_t)n <= _cache_max) return _cache[(size_t)n];

    /* Compute the sequence up to n using the recurrence. */
    for (size_t i = _cache_max + 1; i <= (size_t)n; ++i) {
        seq_t val = 2 * _cache[i - 1];
        val -= 2 * _cache[i - 2];
        val += 2 * _cache[i - 3];
        val -= 2 * _cache[i - 4];
        val += 2 * _cache[i - 5];
        val -= 2 * _cache[i - 6];
        val += 2 * _cache[i - 7];
        val -= 2 * _cache[i - 8];
        val += 2 * _cache[i - 9];
        val -= _cache[i - 10];
        _cache[i] = val;
    }
    _cache_max = (size_t)n;
    return _cache[(size_t)n];
}
