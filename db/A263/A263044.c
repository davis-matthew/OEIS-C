/*
 * A263044  a(1) = a(2) = a(3) = 1; for n>3, a(n) = (a(n-3) + a(n-1))*(a(n-2)
 *          + a(n-3)).
 * https://oeis.org/A263044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (a(n-3) + a(n-1))*(a(n-2) + a(n-3)), a(1)=a(2)=a(3)=1
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 9, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free */
#include <stddef.h>   /* size_t */

seq_t a(idx_t n)
{
    /* trivial case */
    if (n < 1)
        return 0;
    if (n <= 3)
        return 1;

    /* static memoisation table, 1‑based; cache[0] is unused */
    static seq_t *cache = NULL;
    static size_t cap   = 0;

    /* enlarge the table if needed */
    if (cap <= (size_t)n) {
        size_t newcap = (size_t)n + 1;               /* indices 0…n */
        seq_t *newptr = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!newptr)                                 /* allocation failed */
            return 0;
        /* initialise new entries */
        for (size_t i = cap; i < newcap; ++i) {
            if (i >= 1 && i <= 3)
                newptr[i] = 1;
            else
                newptr[i] = 0;                      /* unknown yet */
        }
        cache = newptr;
        cap   = newcap;
    }

    /* compute any missing terms up to n */
    for (size_t i = 4; i <= (size_t)n; ++i) {
        if (cache[i] == (seq_t)0) {                 /* not yet computed */
            seq_t s1 = cache[i - 3] + cache[i - 1];
            seq_t s2 = cache[i - 2] + cache[i - 3];
            cache[i] = s1 * s2;
        }
    }

    return cache[(size_t)n];
}
