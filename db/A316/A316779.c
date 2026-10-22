/*
 * A316779  Expansion of 1 + (1/(1-x) + 1/(1-3*x))*x/2 + (1/(1-x) - 8/(1-2*x)
 *          + 9/(1-3*x))*x^5/2.
 * https://oeis.org/A316779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 5*3^(n-3) – 2^(n-3) (n ≥ 3) a(0) = 1, a(1) = 1, a(2)
 *            = 2 Recurrence: a(n) = 6*a(n-1) – 11*a(n-2) + 6*a(n-3) (n ≥ 6)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static cache to allow repeated calls in any order */
    static seq_t *cache      = NULL;   /* allocated array of terms          */
    static idx_t  cap        = 0;      /* allocated capacity (in terms)      */
    static idx_t  computed   = 0;      /* highest index computed (≥0); 0 when none yet */

    /* ----- Ensure we have the base terms a(0) … a(2) initialized ----- */
    if (computed == 0) {
        /* allocate space for at least 6 terms (up to a(5) explicitly known) */
        cap = 8;
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)1;   /* a(1) */
        cache[2] = (seq_t)2;   /* a(2) */
        /* explicit values for n = 3,4,5 */
        cache[3] = (seq_t)5;   /* a(3) */
        cache[4] = (seq_t)14;  /* a(4) */
        cache[5] = (seq_t)42;  /* a(5) */
        computed = 5;          /* highest computed index now 5            */
    }

    /* If n is already in cache, return it */
    if (n <= computed) return cache[n];

    /* ---- Grow the cache to accommodate index n ---- */
    if (n >= cap) {
        size_t new_cap = cap;
        if (new_cap == 0) new_cap = 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;  /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms from (computed+1) up to n            */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i == 3) {
            cache[3] = (seq_t)5;   /* already set, but keep for safety         */
        } else if (i == 4) {
            cache[4] = (seq_t)14;
        } else if (i == 5) {
            cache[5] = (seq_t)42;
        } else {
            /* Recurrence valid for i ≥ 6 */
            cache[i] = 6 * cache[i-1] - 11 * cache[i-2] + 6 * cache[i-3];
        }
    }

    computed = n;                 /* now we have computed up to n          */
    return cache[n];
}
