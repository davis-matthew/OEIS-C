/*
 * A390646  E.g.f. A(x) satisfies A(x) = 1 + x^2*exp(x)*A(x)^2.
 * https://oeis.org/A390646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   catalan, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A390646
   a(n) = n! * Sum_{k=0..⌊n/2⌋} k^(n-2*k) * Catalan(k) / (n-2*k)!   */

/* Helper functions provided by the build system. */
#include <stdlib.h>
#include "../utils/pow_int.h"
#include "../utils/catalan.h"

static seq_t compute_value(idx_t n)
{
    seq_t sum   = 0;
    seq_t ratio = 1; /* ratio = n! / (n-2k)! for the current k */

    for (idx_t k = 0; k * 2 <= n; ++k) {
        idx_t e = n - 2 * k;                 /* n-2k */
        seq_t power = pow_int((seq_t)k, e);   /* k^(n-2k) */

        seq_t term = ratio;
        term *= catalan(k);
        term *= power;
        sum += term;

        /* Prepare ratio for the next k: multiply by e*(e-1) */
        if (e >= 2) {
            ratio *= (seq_t)e;
            ratio *= (seq_t)(e - 1);
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* Lazy, incremental cache of already computed values. */
    static seq_t *cache        = NULL;   /* allocated array of seq_t */
    static idx_t cache_cap     = 0;      /* allocated array size     */
    static idx_t cache_filled  = 0;      /* number of elements computed */

    /* Ensure the cache is large enough. */
    if (cache_cap < n + 1) {
        size_t needed = (size_t)(n + 1);
        seq_t *newptr = (cache == NULL)
                        ? (seq_t *)malloc(needed * sizeof(seq_t))
                        : (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (newptr == NULL) abort();   /* allocation failure */
        cache = newptr;
        cache_cap = (idx_t)needed;
    }

    /* Compute any missing values up to n. */
    for (idx_t i = cache_filled; i <= n; ++i)
        cache[i] = compute_value(i);

    cache_filled = n + 1;
    return cache[n];
}
