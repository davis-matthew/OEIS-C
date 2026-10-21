/*
 * A259860  a(n+8)+34*a(n+4)+a(n)=0 with a(0)-a(7) as shown.
 * https://oeis.org/A259860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n+8)+34*a(n+4)+a(n)=0 Initial values: a(0)=1, a(1)=5, a(2)=12,
 *            a(3)=12, a(4)=-29, a(5)=-169, a(6)=-408, a(7)=-408 The
 *            recurrence then gives a(n)= -34*a(n-4) - a(n-8) for n ≥ 8
 * Range:     i8 n <= 4, i16 n <= 11, i32 n <= 24, i64 n <= 48, i128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values and its capacity */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;
    /* highest index for which we have computed a value (next index to compute) */
    static idx_t   computed = 0;

    /* first-time initialisation: store the first 8 terms */
    if (cap == 0) {
        cap = 8;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        static const seq_t init[8] = {
            1,      /* a(0) */
            5,      /* a(1) */
            12,     /* a(2) */
            12,     /* a(3) */
            -29,    /* a(4) */
            -169,   /* a(5) */
            -408,   /* a(6) */
            -408    /* a(7) */
        };
        for (size_t i = 0; i < cap; ++i) cache[i] = init[i];
        computed = 8;   /* we have values 0..7 ready */
    }

    /* enlarge cache if necessary */
    if (cap <= n) {
        size_t new_cap = cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);
        /* copy existing computed values */
        for (idx_t i = 0; i < computed; ++i) new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;
        cap   = new_cap;
    }

    /* compute missing values up to index n */
    if (n >= computed) {
        for (idx_t i = computed; i <= n; ++i) {
            /* use the recurrence a(i) = -34*a(i-4) - a(i-8) for i >= 8 */
            if (i >= 8) {
                cache[i] = -((seq_t)34 * cache[i-4] + cache[i-8]);
            } else {
                /* indices 0..7 are already initialised */
                /* this branch is never reached because computed > 7 after init */
                /* but kept for completeness */
            }
        }
        computed = n + 1;
    }

    return cache[n];
}
