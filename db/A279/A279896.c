/*
 * A279896  Number of n X 2 0..2 arrays with no element unequal to a strict
 *          majority of its king-move neighbors, with the exception of exactly
 *          one element, and with new values introduced in order 0
 *          sequentially upwards.
 * https://oeis.org/A279896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) - 4*a(n-3) - 4*a(n-4) for n>7 Initial
 *            values: a(1)=0, a(2)=4, a(3)=6, a(4)=8, a(5)=18, a(6)=36,
 *            a(7)=78, a(8)=160
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Persistent cache of computed terms */
    static seq_t *cache = NULL;          /* array of terms */
    static size_t   cap   = 0;           /* allocated capacity (in terms) */
    static idx_t    last  = 0;           /* highest index computed so far */

    /* Invalid indices are not used.  For safety return 0. */
    if (n < 1) return (seq_t)0;

    /* Ensure cache has room for index n */
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? cap : 64;
        while (new_cap <= (size_t)n) new_cap <<= 1;   /* double until big enough */
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cap   = new_cap;
    }

    /* Initialise the first eight terms once */
    if (last < 8) {
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)8;
        cache[5] = (seq_t)18;
        cache[6] = (seq_t)36;
        cache[7] = (seq_t)78;
        cache[8] = (seq_t)160;
        last    = 8;
    }

    /* Compute any missing terms up to n (recursive formula) */
    for (idx_t i = last + 1; i <= n; ++i) {
        cache[i] = 2 * cache[i - 1] + 3 * cache[i - 2]
                 - 4 * cache[i - 3] - 4 * cache[i - 4];
    }
    last = n;

    return cache[n];
}
