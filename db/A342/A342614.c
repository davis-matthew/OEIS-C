/*
 * A342614  a(0) = 0, a(1) = 1; a(2*n) = 8*a(n), a(2*n+1) = a(n) + a(n+1).
 * https://oeis.org/A342614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 1; a(2*n) = 8*a(n), a(2*n+1) = a(n) + a(n+1)
 *            (OEIS A342614)
 * Range:     u8 n <= 7, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* realloc */

seq_t a(idx_t n)
{
    /* Static cache that stores already computed terms. */
    static seq_t *cache = NULL;   /* cache[i] = a(i) */
    static size_t cap   = 0;      /* allocated size of cache */

    /* Ensure capacity up to n.  Allocate at least two slots to
       safely initialise a(0) and a(1). */
    if (cap <= (size_t)n) {
        size_t newcap = (size_t)n + 1;
        if (newcap < 2) newcap = 2;                /* at least a(0) and a(1) */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failure */
        cache = tmp;
        cap   = newcap;

        /* If the cache was just allocated, initialise the first two terms. */
        if (cap == newcap && (cap == 2 || cap == newcap)) {
            cache[0] = (seq_t)0;
            cache[1] = (seq_t)1;
        }

        /* Compute all missing entries up to n. */
        size_t start = (cap > 2) ? 2 : 2; /* start after the two known terms */
        if (cap == 2) start = 2;          /* all initialised */
        for (size_t i = start; i <= (size_t)n; ++i) {
            if ((i & 1U) == 0U) {                /* even i = 2*k */
                cache[i] = ((seq_t)8) * cache[i >> 1];
            } else {                            /* odd i = 2*k+1 */
                cache[i] = cache[i >> 1] + cache[(i >> 1) + 1];
            }
        }
    }
    return cache[(size_t)n];
}
