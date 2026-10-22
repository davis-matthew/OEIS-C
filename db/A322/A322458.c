/*
 * A322458  Sum of n-th powers of the roots of x^3 - 49*x + 49.
 * https://oeis.org/A322458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 49*a(n-2) - 49*a(n-3) for n > 2 Initial values: a(0) = 3,
 *            a(1) = 0, a(2) = 98
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 10, i64 n <= 21, i128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for arbitrary‑precision or builtin integer builds */
    static seq_t *cache = NULL;
    static size_t cap = 0;        /* current capacity in terms of array size   */
    static size_t computed = 0;   /* lowest non‑cached index (first unfilled) */

    size_t need = (size_t)n + 1;  /* how many entries we need */
    /* --- initialise cache on first use ------------------------------------ */
    if (cache == NULL) {
        /* ensure at least 3 entries to hold the initial values            */
        if (need < 3) need = 3;
#ifdef __cplusplus
        cache = new seq_t[need];
#else
        cache = (seq_t *)malloc(need * sizeof(seq_t));
#endif
        if (cache == NULL) return 0; /* allocation failure (very unlikely) */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)98;
        cap = need;
        computed = 3; /* values 0,1,2 are filled */
    }

    /* --- grow the cache if required --------------------------------------- */
    if (need > cap) {
#ifdef __cplusplus
        seq_t *tmp = new seq_t[need];
        for (size_t i = 0; i < cap; ++i) tmp[i] = cache[i];
        delete[] cache;
        cache = tmp;
#else
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (tmp == NULL) return 0; /* allocation failure */
        cache = tmp;
#endif
        cap = need;
    }

    /* --- compute missing terms up to 'n' ----------------------------------- */
    if (need > computed) {
        for (size_t i = computed; i < need; ++i) {
            /* recurrence: a(i) = 49*a(i-2) - 49*a(i-3)  for i >= 3          */
            cache[i] = (seq_t)49 * cache[i-2] - (seq_t)49 * cache[i-3];
        }
        computed = need;
    }

    return cache[(size_t)n];
}
