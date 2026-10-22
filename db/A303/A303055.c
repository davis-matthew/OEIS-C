/*
 * A303055  Expansion of (1-504*x)^(1/12).
 * https://oeis.org/A303055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 4, i64 n <= 7, i128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1;
   a(n) = (42*(12*n-13)/n) * a(n-1)   for n > 0 */

/* Computes the nth term of OEIS A303055.  The function is thread‑safe
   only for single‑threaded use as required by the test harness.  All
   intermediate products that may exceed the width of seq_t are
   performed in __int128 when seq_t is a fundamental integer type
   (size <= 8 bytes).  For larger seq_t (arbitrary‑precision) the
   provided operators are used directly. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL;   /* cached values            */
    static size_t cache_cap  = 0;      /* allocated capacity       */
    static idx_t  last_computed = 0;   /* highest n cached         */

    /* initialise cache for a(0) on first call */
    if (cache == NULL) {
        /* allocate capacity for at least index 0 */
        size_t new_cap = 1;
        cache = (seq_t*)realloc(NULL, new_cap * sizeof(seq_t));
        if (cache == NULL) abort();
        cache[0] = (seq_t)1;
        cache_cap = new_cap;
        last_computed = 0;
    }

    /* if requested value already computed, return it */
    if (n <= last_computed)
        return cache[n];

    /* ensure capacity for indices up to n */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;            /* double until big enough */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms consecutively */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* compute factor = 42 * (12*i - 13) */
        if (sizeof(seq_t) <= 8) {
            /* use 128‑bit intermediate for small seq_t types */
            __int128 factor = (__int128)42 * (((__int128)12 * (idx_t)i) - 13);
            __int128 prod   = (__int128)cache[i-1] * factor;
            cache[i] = (seq_t)(prod / i);   /* division is exact */
        } else {
            seq_t factor = (seq_t)42 * ((seq_t)12 * (seq_t)i - (seq_t)13);
            cache[i] = cache[i-1] * factor / (seq_t)i; /* exact */
        }
    }

    last_computed = n;
    return cache[n];
}
