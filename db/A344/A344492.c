/*
 * A344492  a(n) = 1 + Sum_{k=0..n-5} binomial(n-4,k) * a(k).
 * https://oeis.org/A344492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{k=0..n-5} binomial(n-4,k) * a(k) Initial values:
 *            a(0)=1, a(1)=1, a(2)=1, a(3)=1, a(4)=1
 * Range:     u8 n <= 11, u16 n <= 16, u32 n <= 25, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

/* Computes A344492.  Results are cached so that calls in any order
   are efficient.  The cache is resized with alloc_seq/free_seq, which
   work for both primitive integer types and arbitrary-precision
   integer classes. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    /* cap == SIZE_MAX indicates that the cache is empty */
    static size_t cap = SIZE_MAX;

    /* Expand cache and compute new values if needed */
    if (n > cap || cache == NULL) {
        size_t new_cap = (size_t)n + 1;          /* length needed */
        seq_t *tmp = alloc_seq(new_cap);         /* allocates new array */
        if (!tmp)                               /* allocation failure */
            return 0;

        /* Copy existing values if cache already existed */
        if (cache != NULL) {
            size_t old_cap = cap;
            for (size_t i = 0; i <= old_cap; ++i)
                tmp[i] = cache[i];
        }

        /* Free the old cache */
        if (cache != NULL)
            free_seq(cache);
        cache = tmp;
        cap = new_cap - 1;                       /* highest index cached */

        /* Determine where to start computing new terms */
        size_t start = (cache == NULL) ? 0 : cap + 1;
        /* However, if cache was just freed, cap was old_cap.
           So start = old_cap+1. */
        if (start == new_cap) /* happens when cache was null */
            start = 0;

        /* Compute terms from 'start' up to 'n' */
        for (idx_t i = start; i <= n; ++i) {
            if (i <= 4) {
                cache[i] = 1;
                continue;
            }
            idx_t m = i - 4;                     /* m = n-4 */
            seq_t sum = 0;
            seq_t bin = 1;                       /* C(m,0) */
            sum += bin * cache[0];               /* k = 0 term */

            /* k = 1 .. m-1 */
            for (idx_t k = 1; k <= m - 1; ++k) {
                /* bin = C(m,k) derived from C(m,k-1) */
                bin = (bin * (m - (k - 1))) / k;
                sum += bin * cache[k];
            }
            cache[i] = 1 + sum;
        }
        /* Update the highest cached index */
        cap = (size_t)n;
    }

    return cache[n];
}
