/*
 * A299523  Number of n X 3 0..1 arrays with every element equal to 1, 3, 4,
 *          5, 6 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A299523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence * a(n) = 2*a(n-1) + 3*a(n-2) + 18*a(n-3) -
 *            36*a(n-4) - 45*a(n-5) * - 51*a(n-6) + 111*a(n-7) + 88*a(n-8) +
 *            94*a(n-9) + 61*a(n-10) * - 89*a(n-11) - 143*a(n-12) -
 *            109*a(n-13) + 52*a(n-14) + 11*a(n-15) * - 7*a(n-16) + 3*a(n-17)
 *            for n > 18 * * Initial values (n = 1 … 18): * a(1) =...
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 21, u64 n <= 40, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* for malloc, realloc */

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL; /* cache[n-1] holds a(n) */
    static idx_t  size    = 0;    /* number of terms stored           */
    static idx_t  capacity = 0;   /* allocated size of cache array    */

    if (n == 0) return (seq_t)0;   /* safety, offset 1 */

    if (size == 0) {
        /* initial 18 terms – indices 0 … 17 correspond to a(1)…a(18) */
        const seq_t init[18] = {
            (seq_t)0,
            (seq_t)4,
            (seq_t)1,
            (seq_t)12,
            (seq_t)35,
            (seq_t)60,
            (seq_t)269,
            (seq_t)873,
            (seq_t)1985,
            (seq_t)7571,
            (seq_t)24138,
            (seq_t)65389,
            (seq_t)225530,
            (seq_t)714174,
            (seq_t)2097771,
            (seq_t)6892581,
            (seq_t)21742964,
            (seq_t)66279272
        };

        /* start with a small capacity, grow as needed */
        capacity = 32;
        cache = (seq_t *)malloc((size_t)capacity * sizeof(seq_t));
        for (idx_t i = 0; i < 18; ++i) cache[i] = init[i];
        size = 18;
    }

    /* extend cache until it contains a(n) */
    while (size < n) {
        /* Grow array if necessary */
        if (size >= capacity) {
            capacity = capacity ? capacity * 2 : 32;
            cache = (seq_t *)realloc(cache, (size_t)capacity * sizeof(seq_t));
        }

        /* idx is the zero‑based index of the term we are about to compute */
        idx_t idx = size;           /* idx = n-1 for the new term */

        /* compute a(n) using the empirical recurrence */
        seq_t pos = cache[idx-1] * ((seq_t)2)
                  + cache[idx-2] * ((seq_t)3)
                  + cache[idx-3] * ((seq_t)18)
                  + cache[idx-7] * ((seq_t)111)
                  + cache[idx-8] * ((seq_t)88)
                  + cache[idx-9] * ((seq_t)94)
                  + cache[idx-10] * ((seq_t)61)
                  + cache[idx-14] * ((seq_t)52)
                  + cache[idx-15] * ((seq_t)11)
                  + cache[idx-17] * ((seq_t)3);

        seq_t neg = cache[idx-4] * ((seq_t)36)
                  + cache[idx-5] * ((seq_t)45)
                  + cache[idx-6] * ((seq_t)51)
                  + cache[idx-11] * ((seq_t)89)
                  + cache[idx-12] * ((seq_t)143)
                  + cache[idx-13] * ((seq_t)109)
                  + cache[idx-16] * ((seq_t)7);

        cache[size] = pos - neg;    /* positive result for all n > 18 */
        ++size;
    }

    return cache[n-1];
}
