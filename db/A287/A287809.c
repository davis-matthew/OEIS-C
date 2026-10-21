/*
 * A287809  Number of septenary sequences of length n such that no two
 *          consecutive terms have distance 2.
 * https://oeis.org/A287809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=7, a(2)=39, a(3)=219, a(4)=1231,
   a(n) = 6*a(n-1) - 13*a(n-3) + 6*a(n-4)   for n > 4 */

#include <stdlib.h>
#include <stdint.h>   /* for abort() */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t  size  = 0;   /* number of elements stored (highest index + 1) */

    if (size <= n) {
        /* grow cache to hold indices up to n */
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();        /* allocation failure */
        cache = tmp;

        /* initialise / compute new entries */
        for (idx_t i = size; i <= n; ++i) {
            if (i == 0)       cache[i] = (seq_t)1;
            else if (i == 1)  cache[i] = (seq_t)7;
            else if (i == 2)  cache[i] = (seq_t)39;
            else if (i == 3)  cache[i] = (seq_t)219;
            else if (i == 4)  cache[i] = (seq_t)1231;
            else {
                cache[i] = ((seq_t)6 * cache[i-1])
                         - ((seq_t)13 * cache[i-3])
                         + ((seq_t)6 * cache[i-4]);
            }
        }
        size = new_size;
    }

    return cache[n];
}
