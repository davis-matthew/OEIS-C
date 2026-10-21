/*
 * A275695  a(0) = a(1) = a(2) = a(3) = a(4) = a(5) = a(6) = 1; for n>6, a(n)
 *          = ( a(n-1)+a(n-3)+a(n-5) )*( a(n-2)+a(n-4)+a(n-6) ) / a(n-7).
 * https://oeis.org/A275695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 11, u64 n <= 13, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A275695
   Recurrence: a(0)=…=a(6)=1;
   for n>6: a(n) = (a(n-1)+a(n-3)+a(n-5))*(a(n-2)+a(n-4)+a(n-6))/a(n-7)
   Declarations:
       seq_t a(idx_t n)
   seq_t and idx_t are supplied by the build.
   No global or main function is required.
*/

#include <stdlib.h>  /* for realloc */

seq_t a(idx_t n)
{
    /* Static cache to store previously computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of allocated elements */

    /* Initialise cache for the first seven terms (indices 0..6). */
    if (cache_len == 0) {
        cache_len = 7;
        cache = (seq_t *)realloc(NULL, cache_len * sizeof(seq_t));
        for (size_t i = 0; i < cache_len; ++i) {
            cache[i] = (seq_t)1;
        }
    }

    /* Expand cache if we need more terms.  Use realloc to preserve
       existing values. */
    size_t required = (size_t)n + 1;
    if (required > cache_len) {
        /* Preserve old length for later loop. */
        size_t old_len = cache_len;
        seq_t *tmp = (seq_t *)realloc(cache, required * sizeof(seq_t));
        if (tmp == NULL) {
            /* realloc failed; keep the old cache and return 0.  This
               situation should never occur under normal build
               conditions. */
            return (seq_t)0;
        }
        cache = tmp;
        cache_len = required;

        /* Compute new terms in order.  All indices below old_len are
           already valid. */
        for (size_t k = old_len; k < required; ++k) {
            seq_t t1 = cache[k-1] + cache[k-3] + cache[k-5];
            seq_t t2 = cache[k-2] + cache[k-4] + cache[k-6];
            cache[k] = (t1 * t2) / cache[k-7];
        }
    }

    return cache[(size_t)n];
}
