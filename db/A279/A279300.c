/*
 * A279300  Number of n X 1 0..2 arrays with no element equal to a strict
 *          majority of its king-move neighbors, with the exception of exactly
 *          two elements, and with new values introduced in order 0
 *          sequentially upwards.
 * https://oeis.org/A279300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 23, u64 n <= 44, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence:
   a(n) = 6*a(n-1) - 6*a(n-2) - 16*a(n-3) + 12*a(n-4) + 24*a(n-5) + 8*a(n-6)  for n>8.
   Initial terms (indexing from 1):
   a(1)=0, a(2)=1, a(3)=0, a(4)=3,
   a(5)=6, a(6)=24, a(7)=72, a(8)=232.
   The function memoises results in a static cache so that calls in any
   order are handled efficiently. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* Safeguard for n <= 0 */

    /* static cache: cache[i] holds a(i+1) */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* initial values for n = 1..8 */
    static const seq_t init[8] = {
        (seq_t)0,
        (seq_t)1,
        (seq_t)0,
        (seq_t)3,
        (seq_t)6,
        (seq_t)24,
        (seq_t)72,
        (seq_t)232
    };

    /* grow cache if necessary */
    if ((size_t)n > cache_len)
    {
        size_t new_len = (size_t)n;
        seq_t *p = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!p) abort();                 /* allocation failure */
        cache = p;

        /* compute missing terms up to n */
        for (idx_t k = (idx_t)(cache_len + 1); k <= n; ++k)
        {
            size_t idx = (size_t)(k - 1);        /* array index for a(k) */

            if (k <= 8)
            {
                cache[idx] = init[(size_t)(k - 1)];
                continue;
            }

            /* recurrence using only seq_t arithmetic (no 128‑bit intermediates) */
            seq_t tmp = (seq_t)6 * cache[idx - 1]
                      - (seq_t)6 * cache[idx - 2]
                      - (seq_t)16 * cache[idx - 3]
                      + (seq_t)12 * cache[idx - 4]
                      + (seq_t)24 * cache[idx - 5]
                      + (seq_t)8  * cache[idx - 6];

            cache[idx] = tmp;
        }

        cache_len = new_len;
    }

    return cache[(size_t)(n - 1)];
}
