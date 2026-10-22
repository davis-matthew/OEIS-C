/*
 * A309331  Expansion of (x+x^3+x^5)/(1-x-3*x^3-x^5).
 * https://oeis.org/A309331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = a(n-1)+3*a(n-3)+a(n-5)+([n==1]+[n==3]+[n==5]), with
 *            a(0)=0, a(1)=1, a(2)=1, a(3)=2, a(4)=5, a(5)=9
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;  /* allocated size (in elements) */
    static idx_t cache_size = 0; /* next unused index */

    /* ensure capacity up to n */
    if (cache_cap <= n) {
        size_t new_cap = (size_t)(n + 1);
        if (new_cap < 16) new_cap = 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failed */
        cache = tmp;
        cache_cap = (idx_t)new_cap;
    }

    /* compute all missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t term = 0;
        if (i >= 1) term += cache[i - 1];
        if (i >= 3) term += (seq_t)3 * cache[i - 3];
        if (i >= 5) term += cache[i - 5];
        if (i == 1 || i == 3 || i == 5) term += (seq_t)1;
        cache[i] = term;
    }
    cache_size = n + 1;
    return cache[n];
}
