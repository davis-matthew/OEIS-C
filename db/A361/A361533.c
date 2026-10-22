/*
 * A361533  Expansion of e.g.f. exp(x^3/(6 * (1-x))).
 * https://oeis.org/A361533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=a(2)=0, a(3)=1, and for n>3 a(n) = 2*(n-1)*a(n-1) -
 *            (n-1)*(n-2)*a(n-2) + binom(n-1,2)*a(n-3) - 2*binom(n-1,3)*a(n-4)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implementation uses memoisation and the O(n) recurrence.  */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc */

/* The sequence type (seq_t) and index type (idx_t) are defined by the build system. */
seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;   /* allocated capacity (in elements) */
    static idx_t  cache_len  = 0;   /* number of elements already computed */

    /* Ensure the cache can store values up to index n. */
    if (cache_cap <= (size_t)(n + 1)) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap <= (size_t)(n + 1))
            new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap != 0) {
            /* Allocation failed – return zero. */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms up to n if needed. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = (seq_t)1;
        } else if (i == 1 || i == 2) {
            val = (seq_t)0;
        } else if (i == 3) {
            val = (seq_t)1;
        } else {
            /* Helper small integers cast to seq_t */
            const seq_t one  = (seq_t)1;
            const seq_t two  = (seq_t)2;
            const seq_t three = (seq_t)3;

            /* Indices as seq_t for multiplication */
            seq_t n1 = (seq_t)(i - 1);
            seq_t n2 = (seq_t)(i - 2);
            seq_t n3 = (seq_t)(i - 3);

            /* Term1 = 2*(n-1)*a[n-1] */
            seq_t term1 = two * n1 * cache[i - 1];

            /* Term2 = (n-1)*(n-2)*a[n-2] */
            seq_t term2 = n1 * n2 * cache[i - 2];

            /* Term3 = binom(n-1,2)*a[n-3] = (n-1)*(n-2)/2 * a[n-3] */
            seq_t term3 = (n1 * n2 / two) * cache[i - 3];

            /* Term4 = 2*binom(n-1,3)*a[n-4]
                       = (n-1)*(n-2)*(n-3)/3 * a[n-4] */
            seq_t term4 = (n1 * n2 * n3 / three) * cache[i - 4];

            val = term1 - term2 + term3 - term4;
        }
        cache[i] = val;
    }

    cache_len = n + 1;
    return cache[n];
}
