/*
 * A391272  Row sums of the triangle A391318, given by T(n,k) = T(n-1,k) +
 *          T(n-1,k-1) + T(n-2,k-1) + 1, with T(0,0) = T(n,-1) = T(n,n+1) = 0.
 * https://oeis.org/A391272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + (n+1), a(0) = 0, a(1) = 2
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 24, u64 n <= 49, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t   max   = (idx_t)-1;   /* indicates uninitialized */
    static size_t  cap   = 0;

    /* Initialize cache on first call */
    if (cache == NULL) {
        cap = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0;          /* allocation failure */
        cache[0] = 0;
        cache[1] = 2;
        max = 1;
    }

    /* Return from cache if already computed */
    if (n <= max) {
        return cache[n];
    }

    /* Expand cache capacity if necessary */
    if ((size_t)n >= cap) {
        size_t new_cap = cap ? cap : 2;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;            /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute terms up to n using the recurrence */
    for (idx_t i = max + 1; i <= n; ++i) {
        cache[i] = 2 * cache[i-1] + cache[i-2] + (seq_t)(i + 1);
    }
    max = n;
    return cache[n];
}
