/*
 * A274032  Sum of n-th powers of the roots of x^3 + 9*x^2 - x - 1.
 * https://oeis.org/A274032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -9*a(n-1) + a(n-2) + a(n-3) Initial values: a(0) = 3,
 *            a(1) = -9, a(2) = 83
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 9, i64 n <= 19, i128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t allocated = 0;   /* size of allocated array      */
    static idx_t filled = 0;      /* number of entries computed   */

    /* Ensure the allocation can hold up to index n and at least 3
       entries for the initial values. */
    size_t new_alloc = (size_t)(n + 1);
    if (new_alloc < 3) new_alloc = 3;
    if (new_alloc > allocated) {
        seq_t *tmp = (seq_t *)realloc(cache, new_alloc * sizeof(seq_t));
        if (!tmp) abort();       /* allocation failed */
        cache = tmp;
        allocated = new_alloc;
    }

    /* Initialise the base cases on first use. */
    if (filled == 0) {
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)-9;
        cache[2] = (seq_t)83;
        filled = (allocated >= 3 ? 3 : allocated);
    }

    /* Compute any missing terms up to n. */
    while (filled <= (idx_t)n) {
        cache[filled] =
            (seq_t)(-9) * cache[filled - 1]
            + cache[filled - 2]
            + cache[filled - 3];
        ++filled;
    }

    return cache[n];
}
