/*
 * A384202  Expansion of e.g.f. -log(1 - 3*x)/(3 * (1 + x)).
 * https://oeis.org/A384202
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n-3)*a(n-1) + 3*(n-1)^2*a(n-2) with a(0) = 0, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* compute the n-th term of OEIS A384202 */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array holding computed terms      */
    static idx_t  last  = 0;      /* highest index already computed   */

    /* Allocate or expand cache to hold index n */
    size_t need = (size_t)(n + 1);
    if (cache == NULL) {
        cache = (seq_t*)malloc(need * sizeof(seq_t));
        if (!cache) abort();                /* allocation failed */
    }
    else if (need > last + 1) {
        seq_t *tmp = (seq_t*)realloc(cache, need * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
    }

    /* initialise the base cases if they are not yet in the cache */
    if (last < 1) {
        cache[0] = 0;
        if (need > 1)
            cache[1] = 1;
    }

    /* compute missing values up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i == 0) {                           /* already initialised */
            continue;
        }
        if (i == 1) {                           /* already initialised */
            last = 1;
            continue;
        }
        seq_t term1 = ((seq_t)(2*i-3)) * cache[i-1];
        seq_t term2 = ((seq_t)3 * (i-1) * (i-1)) * cache[i-2];
        cache[i] = term1 + term2;
        last = i;
    }

    return cache[n];
}
