/*
 * A375292  Expansion of 1/sqrt((1 - x + x^3)^2 + 4*x^4).
 * https://oeis.org/A375292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   n*a(n) = (2*n-1)*a(n-1) - (n-1)*a(n-2) - (2*n-3)*a(n-3) -
 *            2*(n-2)*a(n-4) - (n-3)*a(n-6) with initial values: a(0)=1,
 *            a(1)=1, a(2)=1, a(3)=0, a(4)=-3, a(5)=-8
 * Range:     i8 n <= 8, i16 n <= 14, i32 n <= 33, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap = 0;

    /* initialise cache with the first 6 terms */
    if (cap == 0) {
        cap = 6;                     /* indices 0 .. 5 */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;        /* allocation failure */

        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 0;
        cache[4] = (seq_t)(-3);
        cache[5] = (seq_t)(-8);
    }

    /* fast return for already cached values */
    if (n < cap)
        return cache[n];

    /* extend cache up to index n */
    if (n >= cap) {
        size_t new_cap = n + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0;    /* allocation failure */
        cache = new_cache;

        for (size_t i = cap; i <= n; ++i) {
            seq_t term =
                ((seq_t)(2 * i - 1)) * cache[i - 1]
              - ((seq_t)(i - 1))     * cache[i - 2]
              - ((seq_t)(2 * i - 3)) * cache[i - 3]
              - ((seq_t)2)           * ((seq_t)(i - 2)) * cache[i - 4]
              - ((seq_t)(i - 3))     * cache[i - 6];
            cache[i] = term / ((seq_t)i);
        }
        cap = new_cap;
    }

    return cache[n];
}
