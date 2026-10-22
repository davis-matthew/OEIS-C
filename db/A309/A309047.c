/*
 * A309047  Expansion of Product_{k>=0} (1 + x^(2^k) - x^(3*2^k)).
 * https://oeis.org/A309047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = a(1) = 1; a(2*n) = a(n); a(2*n+1) = a(n) - a(n-1)
 * Range:     i8 n <= 109, i16 n <= 109, i32 n <= 109, i64 n <= 109,
 *            i128 n <= 109, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* malloc, realloc */

seq_t a(idx_t n) {
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;

    /* initialise cache when first called */
    if (cache == NULL) {
        cap = 256;            /* initial capacity (enough for early values) */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0;      /* out of memory – unlikely for tests */
        cache[0] = 1;
        cache[1] = 1;
    }

    /* grow cache if necessary */
    if (cap <= (size_t)n) {
        size_t new_cap = cap;
        if (new_cap == 0) new_cap = 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;  /* out of memory */
        cache = tmp;
        cap = new_cap;
    }

    /* compute values up to n (iteratively) */
    for (idx_t i = 2; i <= n; ++i) {
        if ((i & 1) == 0) {                /* i even: a(i) = a(i/2) */
            idx_t m = i >> 1;
            cache[i] = cache[m];
        } else {                           /* i odd:  a(i) = a(i/2) - a(i/2-1) */
            idx_t m = i >> 1;
            cache[i] = cache[m] - cache[m-1];
        }
    }

    return cache[n];
}
