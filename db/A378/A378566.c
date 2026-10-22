/*
 * A378566  a(n) = Sum_{k=0..n} binomial(n+k-1,k) * binomial(n+2*k-1,n-k).
 * https://oeis.org/A378566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n+k-1,k) * binomial(n+2*k-1,n-k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t a, idx_t b) */

seq_t a(idx_t n)
{
    /* Static cache for already computed values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* current allocated capacity */
    static idx_t  max_n = (idx_t)-1;  /* highest n that has been computed */

    /* Allocate or grow cache to hold indices 0..n */
    if (n >= cap) {
        size_t new_cap = cap ? cap * 2 : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        /* cast the result of realloc to seq_t* to satisfy C++ */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failed – return 0 as fallback */
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise a(0) once */
    if (max_n == (idx_t)-1) {
        cache[0] = (seq_t)1;
        max_n = 0;
    }

    /* Return cached value if already known */
    if (n <= max_n)
        return cache[n];

    /* Compute missing terms incrementally up to n */
    for (idx_t m = max_n + 1; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k <= m; ++k) {
            seq_t c1 = binomial(m + k - 1, k);
            seq_t c2 = binomial(m + 2 * k - 1, m - k);
            sum += c1 * c2;
        }
        cache[m] = sum;
    }
    max_n = n;
    return cache[n];
}
