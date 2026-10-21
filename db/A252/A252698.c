/*
 * A252698  Number of strings of length n over a 5-letter alphabet that do not
 *          begin with a palindrome.
 * https://oeis.org/A252698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, walk
 * Formula:   a(n) = number of strings of length n over a 5‑letter alphabet
 *            that do not begin with a palindrome. a(0) = 0, a(1) = 5.
 *            Recurrence (derived in the explanation): For n ≥ 2 : a(n) = 5 *
 *            a(n−1) – a( floor((n−1)/2) + 1 )
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cached = 0;

    /* Ensure cache is large enough */
    if (n >= cached) {
        size_t newsize = n + 1;
        seq_t *newcache = (seq_t*)malloc(newsize * sizeof(seq_t));
        if (!newcache) return 0; /* allocation failed; caller will handle */

        /* copy existing values */
        for (size_t i = 0; i < cached; ++i)
            newcache[i] = cache[i];

        /* initialise new entries */
        if (cached == 0) {
            newcache[0] = 0;            /* a(0) = 0 */
            if (newsize > 1) newcache[1] = 5; /* a(1) = 5 */
        }
        for (size_t i = cached; i < newsize; ++i) {
            if (i == 0) {
                newcache[0] = 0;
            } else if (i == 1) {
                newcache[1] = 5;
            } else {
                /* recurrence: a(i) = 5*a(i-1) - a( floor((i-1)/2)+1 ) */
                idx_t idx = (i - 1) / 2;  /* floor division */
                newcache[i] = 5 * newcache[i - 1] - newcache[idx + 1];
            }
        }

        free(cache);
        cache = newcache;
        cached = newsize;
    }

    return cache[n];
}
