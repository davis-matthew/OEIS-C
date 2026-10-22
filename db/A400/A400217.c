/*
 * A400217  Scaled y-coordinates of the vertices of a Boerdijk-Coxeter helix
 *          starting at a tetrahedron with vertices (-1,-1,-1), (1, -1, 1),
 *          (-1, 1, 1), and (1, 1, -1).
 * https://oeis.org/A400217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 2*a(n-1) + 6*a(n-2) + 18*a(n-3) - 81*a(n-4) for n >= 7
 *            Initial values: a(0) = -1, a(1) = 1, a(2) = -1, a(3) = 1, a(4) =
 *            5, a(5) = 1, a(6) = 77
 * Range:     i8 n <= 6, i16 n <= 11, i32 n <= 20, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;      /* array of seq_t values */
    static idx_t len   = 0;          /* number of elements stored (== size of valid data) */
    static idx_t cap   = 0;          /* capacity of allocated array */

    /* Grow the cache if necessary */
    if (n + 1 > cap) {
        idx_t new_cap = cap ? cap << 1 : 16;
        while (new_cap <= n) new_cap <<= 1;
        /* realloc may be NULL if out of memory; we skip error handling for brevity */
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cap   = new_cap;
    }

    /* Initialise the first seven terms on the first call */
    if (len < 7) {
        /* ensure we have enough space */
        if (cap < 7) {
            free(cache);
            cache = (seq_t*)malloc(7 * sizeof(seq_t));
            cap   = 7;
        }
        cache[0] = -1;
        cache[1] =  1;
        cache[2] = -1;
        cache[3] =  1;
        cache[4] =  5;
        cache[5] =  1;
        cache[6] = 77;
        len = 7;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = len; i <= n; ++i) {
        /* Recurrence: a(i) = 2*a(i-1) + 6*a(i-2) + 18*a(i-3) - 81*a(i-4)   (valid for i >= 7) */
        cache[i] = 2 * cache[i - 1] + 6 * cache[i - 2] + 18 * cache[i - 3] - 81 * cache[i - 4];
    }

    len = (n + 1 > len) ? (n + 1) : len;

    return cache[n];
}
