/*
 * A372002  G.f. A(x) satisfies A(x) = ( 1 + 4*x*(1 + x*A(x)) )^(1/2).
 * https://oeis.org/A372002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 17, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A372002
 * Recurrence: n*a(n) + 2*(2*n-3)*a(n-1) + 4*(n-6)*a(n-4) = 0
 * Base values: a(0)=1, a(1)=2, a(2)=0, a(3)=4
 * Memoised with a static cache, so calls in any order are O(1) per call. */

#include <stdlib.h>  /* malloc, realloc */

seq_t a(idx_t n) {
    /* static cache of computed terms */
    static seq_t *cache = NULL;      /* pointer to cached sequence values */
    static size_t max_computed = 0;  /* highest index computed so far */

    /* initialise cache with first four terms on first call */
    if (!cache) {
        /* allocate space for indices 0..3 */
        cache = (seq_t *)malloc(4 * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failure: return 0 */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)4;
        max_computed = 3;
    }

    /* if the value is already computed, return it */
    if ((size_t)n <= max_computed)
        return cache[n];

    /* extend the cache to accommodate up to n */
    size_t new_cap = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return (seq_t)0;  /* allocation failure */
    cache = tmp;

    /* compute new terms using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* term1 = 2*(2*i-3)*a(i-1) */
        seq_t term1 = 2 * ((seq_t)(2 * i) - 3) * cache[i - 1];
        /* term2 = 4*(i-6)*a(i-4) */
        seq_t term2 = 4 * ((seq_t)i - 6) * cache[i - 4];
        /* a(i) = -(term1 + term2) / i */
        seq_t numer = -(term1 + term2);
        cache[i] = numer / (seq_t)i;
    }

    max_computed = (size_t)n;
    return cache[n];
}
