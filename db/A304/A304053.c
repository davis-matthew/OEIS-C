/*
 * A304053  Number of nX3 0..1 arrays with every element unequal to 1, 2, 4 or
 *          5 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A304053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is given by the empirical linear recurrence for n > 18:
 *            a(n) = a(n-1) + 2*a(n-2) + 7*a(n-3) - 3*a(n-4) - 11*a(n-5) -
 *            21*a(n-6) - 8*a(n-7) + 13*a(n-8) + 25*a(n-9) + 22*a(n-10) +
 *            4*a(n-11) + 3*a(n-12) - 6*a(n-13) - 6*a(n-14) - 9*a(n-15) with
 *            initial values (offset 1) given below
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 55, u128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>    /* for size_t */
#include <stdlib.h>    /* for malloc, realloc */

/* Initial terms a(1)..a(18) from OEIS A304053. */
static const seq_t _init_values[19] = {
    0,        /* dummy for index 0 */
    1, 11, 6, 18, 32, 60, 124, 288, 598, 1266,
    2910, 6362, 13924, 31578, 70292, 156670, 353098, 791762
};

/* Simple memoisation cache for already computed terms. */
static seq_t * _cache = NULL;  /* array indexed by n, 1‑based */
static idx_t   _cache_len = 0; /* largest n cached, 0 if no cache */

seq_t a(idx_t n)
{
    if (n < 1) {
        return 0;   /* undefined for n = 0 or negative */
    }

    /* Initialise cache if this is the first call. */
    if (_cache_len == 0) {
        _cache_len = 18;   /* we have a(1)..a(18) */
        _cache = (seq_t*)malloc((_cache_len + 1) * sizeof(seq_t));
        for (size_t i = 0; i <= _cache_len; ++i) {
            _cache[i] = _init_values[i];
        }
    }

    /* Extend cache if necessary. */
    if ((idx_t)_cache_len < n) {
        size_t new_len = (size_t)n + 1;
        _cache = (seq_t*)realloc(_cache, new_len * sizeof(seq_t));
        for (idx_t i = (idx_t)_cache_len + 1; i <= n; ++i) {
            /* Apply the 15‑term empirical recurrence. */
            seq_t val = _cache[i-1];
            val += 2 * _cache[i-2];
            val += 7 * _cache[i-3];
            val -= 3 * _cache[i-4];
            val -= 11 * _cache[i-5];
            val -= 21 * _cache[i-6];
            val -= 8 * _cache[i-7];
            val += 13 * _cache[i-8];
            val += 25 * _cache[i-9];
            val += 22 * _cache[i-10];
            val += 4  * _cache[i-11];
            val += 3  * _cache[i-12];
            val -= 6  * _cache[i-13];
            val -= 6  * _cache[i-14];
            val -= 9  * _cache[i-15];
            _cache[i] = val;
        }
        _cache_len = n;
    }

    return _cache[n];
}
