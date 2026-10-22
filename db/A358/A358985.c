/*
 * A358985  a(n) is the number of numbers of the form k + reverse(k) for at
 *          least one n-digit number k.
 * https://oeis.org/A358985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A358985:
   a(1)=10, a(2)=18,
   a(2*n)=19*a(2*n-2)   for n>1,
   a(2*n+1)=10*a(2*n)   for n>0. */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache[0] is unused; index 1..n are stored. */
    static seq_t *cache = NULL;
    static idx_t  cap   = 0;

    if (n == 0)           /* offset starts at 1 */
        return 0;

    /* enlarge cache if necessary */
    if (n >= cap) {
        idx_t newcap = n + 1;               /* allocate slots 0..n */
        seq_t *tmp = (seq_t *)realloc(cache,
                                       (size_t)newcap * sizeof(seq_t));
        if (!tmp) {                         /* allocation failed */
            free(cache);
            cache = NULL;
            cap   = 0;
            return 0;
        }
        cache = tmp;
        /* initialise any newly allocated entries to zero */
        for (idx_t i = cap; i < newcap; ++i)
            cache[i] = 0;
        cap = newcap;
    }

    /* compute values up to n on demand */
    for (idx_t i = 1; i <= n; ++i) {
        if (cache[i] != 0)                 /* already computed */
            continue;

        if (i == 1) {
            cache[i] = 10;
        } else if (i == 2) {
            cache[i] = 18;
        } else if ((i & 1) == 0) {          /* even i = 2*n */
            cache[i] = 19 * cache[i - 2];
        } else {                            /* odd i = 2*n+1 */
            cache[i] = 10 * cache[i - 1];
        }
    }
    return cache[n];
}
