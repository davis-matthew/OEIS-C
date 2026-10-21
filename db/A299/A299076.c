/*
 * A299076  Number of nX3 0..1 arrays with every element equal to 0, 1, 2, 3,
 *          4 or 7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A299076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A299076
 * Recurrence (valid for n > 11):
 *   a(n) = 5*a(n-1) + 4*a(n-2) + 7*a(n-3)
 *          - 26*a(n-4) - 28*a(n-5) - 18*a(n-6) - 16*a(n-7)
 *          + 12*a(n-8) +   8*a(n-9) + 16*a(n-10)
 * Initial values:
 *   a(1)=4, a(2)=31, a(3)=163, a(4)=927,
 *   a(5)=5331, a(6)=30535, a(7)=175286,
 *   a(8)=1006611, a(9)=5780036,
 *   a(10)=33191586, a(11)=190603429.
 */

#include <stddef.h>
#include <stdlib.h>

/* External types supplied by the build system.
 * Do not redeclare them.                                  */
extern ptrdiff_t __attribute__((unused)) dummy; /* silence unused warning */

seq_t a(idx_t n)
{
    /* Cache of computed values.
     * cache[0] is unused; cache[1] holds a(1), etc.    */
    static seq_t *cache = NULL;
    static size_t cached = 0;  /* highest index stored + 1                */

    /* trivial/fallback case */
    if (n == 0)
        return (seq_t)0;

    /* Initialise the cache with the first 11 values.  */
    if (!cached) {
        cached = 12;                                 /* indices 0..11 */
        cache = (seq_t *)malloc(cached * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;   /* allocation failed */
        static const seq_t init[12] = {
            0ULL,          /* unused placeholder */
            4ULL,          /* a(1) */
            31ULL,         /* a(2) */
            163ULL,        /* a(3) */
            927ULL,        /* a(4) */
            5331ULL,       /* a(5) */
            30535ULL,      /* a(6) */
            175286ULL,     /* a(7) */
            1006611ULL,    /* a(8) */
            5780036ULL,    /* a(9) */
            33191586ULL,   /* a(10) */
            190603429ULL   /* a(11) */
        };
        for (size_t i = 0; i < cached; ++i)
            cache[i] = init[i];
    }

    /* Extend the cache up to n if necessary.          */
    if ((size_t)n + 1 > cached) {
        size_t newsize = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;   /* realloc failed */
        cache = tmp;
        for (size_t i = cached; i <= (size_t)n; ++i) {
            if (i <= 11)          /* already initialised   */
                continue;
            /* recurrence for i > 11 */
            cache[i] =
                5ULL * cache[i-1] + 4ULL * cache[i-2] + 7ULL * cache[i-3]
              - 26ULL * cache[i-4] - 28ULL * cache[i-5]
              - 18ULL * cache[i-6] - 16ULL * cache[i-7]
              + 12ULL * cache[i-8] +  8ULL * cache[i-9]
              + 16ULL * cache[i-10];
        }
        cached = newsize;
    }

    return cache[(size_t)n];
}
