/*
 * A309878  The real part of b(n) where b(n) = (n + b(n-1)) * (1 + i) with
 *          b(-1)=0; i = sqrt(-1).
 * https://oeis.org/A309878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4*a(n-1) - 7*a(n-2) + 6*a(n-3) - 2*a(n-4) with initial
 *            values a(0)=0, a(1)=1, a(2)=2, a(3)=1 (Real part of b(n) defined
 *            by b(n) = (n + b(n-1))*(1+i) with b(-1)=0)
 * Range:     i8 n <= 12, i16 n <= 28, i32 n <= 44, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache: computed values and their capacity. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t max_computed = 0;

    /* Initialise cache on first call. */
    if (cache == NULL) {
        cache_cap = 8;                     /* initial capacity */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) {                       /* allocation failure */
            /* In a real program you might handle this differently. */
            abort();
        }
        /* Base cases for n = 0..3 */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)1;
        max_computed = 3;
    }

    /* Grow cache if necessary. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;   /* double until big enough */
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {                               /* realloc failed */
            abort();
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Fast return if the requested value is already computed. */
    if ((size_t)n <= max_computed) {
        return cache[(size_t)n];
    }

    /* Compute the sequence up to n using the recurrence. */
    for (size_t i = max_computed + 1; i <= (size_t)n; ++i) {
        seq_t term = (seq_t)4 * cache[i - 1]
                   - (seq_t)7 * cache[i - 2]
                   + (seq_t)6 * cache[i - 3]
                   - (seq_t)2 * cache[i - 4];
        cache[i] = term;
    }
    max_computed = (size_t)n;

    return cache[(size_t)n];
}
