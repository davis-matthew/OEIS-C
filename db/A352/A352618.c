/*
 * A352618  Squares that are 7-smooth.
 * https://oeis.org/A352618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002473(n)^2 (square of the n-th 7‑smooth number)
 * Range:     u8 n <= 13, u16 n <= 76, u32 n <= 613, u64 n <= 6623,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for generated 7‑smooth numbers (ascending order) */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity in elements   */
    static size_t len   = 0;   /* number of elements actually present */
    /* indices into the cache for each multiplier */
    static idx_t i2 = 0, i3 = 0, i5 = 0, i7 = 0;

    if (n == 0)               /* off‑by‑offset guard – value arbitrary */
        return 0;

    /* initialise the cache on first call */
    if (cache == NULL) {
        cap   = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();        /* cannot proceed without memory */
        cache[0] = 1;               /* first 7‑smooth number */
        len = 1;
        i2 = i3 = i5 = i7 = 0;
    }

    /* grow the cache until it contains at least n 7‑smooth numbers */
    while (len < n) {
        /* double capacity if necessary */
        if (len >= cap) {
            size_t new_cap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cap = new_cap;
        }

        /* candidate multiples of the four primes */
        seq_t c2 = cache[i2] * 2;
        seq_t c3 = cache[i3] * 3;
        seq_t c5 = cache[i5] * 5;
        seq_t c7 = cache[i7] * 7;

        /* find the smallest candidate */
        seq_t next = c2;
        if (c3 < next) next = c3;
        if (c5 < next) next = c5;
        if (c7 < next) next = c7;

        /* append the new 7‑smooth number */
        cache[len++] = next;

        /* advance all indices that produced this minimal value */
        if (c2 == next) ++i2;
        if (c3 == next) ++i3;
        if (c5 == next) ++i5;
        if (c7 == next) ++i7;
    }

    /* return the square of the n-th 7‑smooth number */
    seq_t val = cache[n - 1];
    return val * val;
}
