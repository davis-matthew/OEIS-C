/*
 * A293532  Expansion of e.g.f. exp(x/(x^2 - 1)).
 * https://oeis.org/A293532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 11, i64 n <= 19, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: n*(n+1)*(n+2)*(n+3)*a(n) + (n+3)*(n+2)*a(n+1)
   - 2*(n+3)*(n+2)*a(n+2) + a(n+3) + a(n+4) = 0
   with initial values a(0)=1, a(1)=-1, a(2)=1, a(3)=-7, a(4)=25. */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t computed = 0;

    /* initialise cache with first 5 terms */
    if (!cache) {
        cache_len = 5;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)-7;
        cache[4] = (seq_t)25;
        computed = 4;
    }

    /* grow the cache if needed */
    if ((n + 1U) > cache_len) {
        size_t new_len = (size_t)(n + 1U);
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                  /* allocation failure */
        cache = tmp;
        cache_len = (idx_t)new_len;
    }

    /* compute new terms up to n, if needed */
    if (n > computed) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            idx_t k = i - 4;                       /* n in the recurrence */
            seq_t nk = (seq_t)k;

            /* term1 = n*(n+1)*(n+2)*(n+3)*a(n) */
            seq_t term1 = nk * (nk + 1);
            term1 = term1 * (nk + 2);
            term1 = term1 * (nk + 3);
            term1 = term1 * cache[i - 4];

            /* term2 = (n+3)*(n+2)*a(n+1) */
            seq_t term2 = (nk + 3) * (nk + 2);
            term2 = term2 * cache[i - 3];

            /* term3 = 2*(n+3)*(n+2)*a(n+2) */
            seq_t term3 = (nk + 3) * (nk + 2);
            term3 = term3 * cache[i - 2];
            term3 = term3 * 2;

            /* a(n+4) = -(term1 + term2 - term3 + a(n+3)) */
            seq_t val = term1 + term2 - term3 + cache[i - 1];
            cache[i] = -val;
        }
        computed = n;
    }

    return cache[n];
}
