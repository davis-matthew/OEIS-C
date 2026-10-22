/*
 * A355070  G.f.: Sum_{n>=0} a(n)*x^n/(n!*3^(n*(n-1)/2)) = log( Sum_{n>=0}
 *          x^n/(n!*3^(n*(n-1)/2)) ).
 * https://oeis.org/A355070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 0; for n>0: a(n) = 1 - Σ_{k=1}^{n-1} 3^{k*(n-k)} *
 *            binomial(n-1, k) * a(n-k)
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 6, i64 n <= 8, i128 n <= 12,
 *            big any n
 * Helpers:   binomial, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/binomial.h"   /* binomial(idx_t n, idx_t k) */
#include "../utils/pow3.h"       /* pow3(idx_t exp) */

seq_t a(idx_t n)
{
    /* Handle base cases */
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;

    /* Lazy memoisation cache */
    static seq_t *cache = NULL;   /* array of a[0..cache_cap-1] */
    static size_t cache_cap = 0;  /* allocated number of elements */
    static idx_t cache_to  = 0;   /* highest index computed so far */

    /* Ensure we can store up to n */
    size_t required = (size_t)n + 1;
    if (cache == NULL) {
        /* initial allocation (at least up to index 1) */
        if (required < 2)
            required = 2;
        cache = (seq_t*)malloc(required * sizeof(seq_t));
        if (!cache)
            return 0;              /* allocation failure (unlikely in tests) */
        cache[0] = 0;
        cache[1] = 1;
        cache_cap = required;
        cache_to  = 1;
    } else if (required > cache_cap) {
        seq_t *tmp = (seq_t*)realloc(cache, required * sizeof(seq_t));
        if (!tmp)
            return 0;              /* allocation failure */
        cache = tmp;
        cache_cap = required;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_to + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k < i; ++k) {
            idx_t exp = k * (i - k);
            seq_t term = pow3(exp);                 /* 3^exp */
            term = term * binomial(i - 1, k);      /* * binomial(i-1,k) */
            term = term * cache[i - k];           /* * a(i-k) */
            sum = sum + term;
        }
        cache[i] = 1 - sum;                        /* a(i) */
    }
    cache_to = n;
    return cache[n];
}
