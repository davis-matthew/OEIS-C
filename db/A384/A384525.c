/*
 * A384525  Expansion of e.g.f. 5/(6 - exp(5*x)).
 * https://oeis.org/A384525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A384525
 *
 * Recurrence: a(0) = 1
 *              a(n) = Σ_{k=1..n} 5^(k-1) * binomial(n,k) * a(n-k)
 *
 * This implementation memoises all computed terms in a static dynamic
 * array.  The array is grown as needed and each new entry is filled
 * with the value from the recurrence.  All arithmetic is performed
 * with the user‑supplied type `seq_t` and the helper function
 * `binomial(idx_t a, idx_t b)`. */

#include <stddef.h>        /* size_t */
#include <stdlib.h>        /* malloc, realloc, free */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Static cache of computed terms. */
    static seq_t *cache      = NULL;   /* array of terms */
    static idx_t cache_size  = 0;      /* allocated size (elements)   */
    static idx_t cache_last  = 0;      /* highest index that has been computed */

    /* Initialise cache for a(0) if it hasn't been done yet. */
    if (cache_size == 0) {
        cache_size = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) return 0;          /* allocation failed */
        cache[0] = 1;                  /* a(0) */
        cache_last = 0;
    }

    /* Grow the allocated array if necessary. */
    if (n + 1 > cache_size) {
        idx_t new_size = cache_size * 2;
        while (new_size <= n) new_size <<= 1;   /* at least n+1 elements */

        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp) return 0;                    /* allocation failed */
        cache      = tmp;
        cache_size = new_size;
    }

    /* If the requested term is already computed, return it. */
    if (n <= cache_last) {
        return cache[n];
    }

    /* Compute all missing terms up to n. */
    for (idx_t m = cache_last + 1; m <= n; ++m) {
        seq_t sum  = 0;
        seq_t pow5 = 1;                       /* 5^(k-1) for k=1 */

        for (idx_t k = 1; k <= m; ++k) {
            seq_t term = pow5;
            term       = term * binomial(m, k);
            term       = term * cache[m - k];
            sum        = sum + term;
            pow5       = pow5 * 5;
        }
        cache[m] = sum;
    }

    cache_last = n;
    return cache[n];
}
