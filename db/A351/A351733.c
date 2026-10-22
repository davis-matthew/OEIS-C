/*
 * A351733  Expansion of e.g.f. exp( 2 * x * (exp(x) - 1) ).
 * https://oeis.org/A351733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/2)} 2^k * Stirling2(n-k,k)/(n-k)!
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Compute OEIS A351733 (exp(2*x*(exp(x)-1))) */
seq_t a(idx_t n)
{
    /* memoized cache for already-computed terms */
    static seq_t *cache = NULL;
    static idx_t cap = 0;        /* current capacity of cache */

    /* initialise for a(0) */
    if (cache == NULL) {
        cache = seq_alloc(1);    /* one element for a[0] */
        cache[0] = 1;            /* a(0) = 1 */
        cap = 1;
    }

    /* extend cache up to n if necessary */
    if (n >= cap) {
        idx_t new_cap = n + 1;
        cache = seq_realloc(cache, cap, new_cap);

        for (idx_t i = cap; i <= n; ++i) {
            seq_t sum = 0;
            seq_t binom = 1;   /* binom(i-1,0) for k=1 */

            for (idx_t k = 2; k <= i; ++k) {
                /* update binom = binom(i-1, k-1) from previous value   */
                binom = binom * (seq_t)(i - k + 1) / (seq_t)(k - 1);

                seq_t term = cache[i - k];
                term *= (seq_t)k;
                term *= 2;
                term *= binom;

                sum += term;
            }
            cache[i] = sum;
        }
        cap = new_cap;
    }
    return cache[n];
}
