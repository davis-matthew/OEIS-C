/*
 * A396473  Expansion of g.f. (1 - 4*x^2)/(1 - 2*x - 4*x^2 + 2*x^3).
 * https://oeis.org/A396473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 2, a(2) = 4; a(n) = 2*a(n-1) + 4*a(n-2) -
 *            2*a(n-3) for n >= 3 (recurrence derived from the generating
 *            function)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Cached values for fast repeated queries */
static seq_t *cache        = (seq_t *)0;   /* pointer to an array of seq_t */
static size_t cache_capacity = 0;          /* number of elements allocated */
static size_t cache_computed = 0;          /* number of elements already stored */

/* Return a(n) for any n >= 0, independently of call order */
seq_t a(idx_t n)
{
    /* ensure we have room for at least n+1 entries */
    size_t need = (size_t)n + 1;
    if (cache_capacity < need) {
        size_t new_cap = cache_capacity ? cache_capacity << 1 : 1;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);        /* allocation failed */
        cache          = tmp;
        cache_capacity = new_cap;
    }

    /* compute any missing terms up to n */
    for (idx_t i = (idx_t)cache_computed; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1) {
            cache[1] = (seq_t)2;
        } else if (i == 2) {
            cache[2] = (seq_t)4;
        } else {
            /* a(i) = 2*a(i-1) + 4*a(i-2) - 2*a(i-3) */
            seq_t val = (seq_t)2 * cache[i-1];
            val += (seq_t)4 * cache[i-2];
            val -= (seq_t)2 * cache[i-3];
            cache[i] = val;
        }
    }
    cache_computed = need;
    return cache[n];
}
