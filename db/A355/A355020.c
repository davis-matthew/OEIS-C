/*
 * A355020  a(n) = (-1)^n * A000045(n) + 1.
 * https://oeis.org/A355020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*a(n-2) - a(n-3); a(0)=1, a(1)=0, a(2)=2
 * Range:     i8 n <= 11, i16 n <= 23, i32 n <= 46, i64 n <= 92, i128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cacheSize = 0;
    static idx_t computed_upto = 2;

    /* ensure cache is allocated and initialized for indices 0..2 */
    if (cache == NULL) {
        cacheSize = 16;
        cache = (seq_t *)realloc(NULL, cacheSize * sizeof(seq_t));
        if (cache == NULL) abort();  /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)2;
    }

    /* grow cache if needed */
    if (n >= cacheSize) {
        idx_t newSize = n + 1;
        if (newSize < 2 * cacheSize) newSize = 2 * cacheSize;
        seq_t *tmp = (seq_t *)realloc(cache, newSize * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cacheSize = newSize;
    }

    /* extend computed values up to n */
    if (n > computed_upto) {
        for (idx_t i = computed_upto + 1; i <= n; ++i) {
            /* a(i) = 2*a(i-2) - a(i-3) = a(i-2)+a(i-2)-a(i-3) */
            seq_t two_prev = cache[i - 2] + cache[i - 2];
            cache[i] = two_prev - cache[i - 3];
        }
        computed_upto = n;
    }

    return cache[n];
}
