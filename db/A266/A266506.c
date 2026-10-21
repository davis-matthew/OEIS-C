/*
 * A266506  a(n) = 2*a(n-4) + a(n-8) for n >= 8.
 * https://oeis.org/A266506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = 2*a(n-4) + a(n-8) (n ≥ 8), a(0)=2, a(1)=-1, a(2)=2,
 *            a(3)=1, a(4)=1, a(5)=3, a(6)=3, a(7)=5
 * Range:     i8 n <= 22, i16 n <= 48, i32 n <= 98, i64 n <= 198,
 *            i128 n <= 400, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Recurrence based memoized evaluation */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  next  = 0;            /* index of first unset element */

    /* ensure we have enough space for n+1 elements */
    size_t required = (size_t)n + 1;
    if (cap < required) {
        size_t newcap = required;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache = tmp;
        cap   = newcap;
    }

    /* initial values */
    static const seq_t init[8] = { 2, -1, 2, 1, 1, 3, 3, 5 };
    static const seq_t two   = 2;

    /* initialise first eight entries if they are needed */
    for (size_t i = 0; i < 8 && i < required; ++i)
        cache[i] = init[i];

    /* compute missing terms up to n */
    for (idx_t i = next; i <= n; ++i) {
        if (i < 8) {
            /* already initialised */
        } else {
            cache[i] = cache[i-4] * two + cache[i-8];
        }
    }

    /* remember next position to fill */
    next = n + 1;
    return cache[n];
}
