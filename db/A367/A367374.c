/*
 * A367374  Expansion of the e.g.f. (exp(x) / (5 - 4*exp(x)))^(2/5).
 * https://oeis.org/A367374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0) = 1
 *          a(n) = 2*a(n-1) + 4*Σ_{k=1}^{n-1} C(n-1,k)*a(n-k)   (n ≥ 1)
 *
 * The implementation memoises previously computed terms so that
 * independent calls to a(idx_t n) work in any order.
 */

#include <stdlib.h>

/* The function definition required by the build system */
seq_t a(idx_t n)
{
    /* static cache of already computed values                */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache with a[0] = 1 on first entry           */
    if (cache == NULL) {
        cache_size = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        cache[0] = (seq_t)1;
    }

    /* extend cache if n is beyond the current maximum          */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (new_cache == NULL) { /* allocation failure – abort */
            /* a defensive fallback: return 0 (should never happen) */
            return (seq_t)0;
        }
        cache = new_cache;

        /* compute all new terms from the current size up to n     */
        for (idx_t i = cache_size; i < new_size; ++i) {
            seq_t sum   = (seq_t)0;
            seq_t binom = (seq_t)1;   /* C(i-1,0) */

            /* iterate through k = 1 .. i-1
               (k corresponds to the index of a[k] in the sum)    */
            for (idx_t k = 1; k < i; ++k) {
                sum += binom * cache[k];
                /* update binomial coefficient to C(i-1,k)       */
                seq_t mult = (seq_t)(i - k);          /* (i-1)- (k-1) + 1 = i-k */
                binom = binom * mult / (seq_t)k;      /* exact division  */
            }

            cache[i] = (seq_t)2 * cache[i-1] + (seq_t)4 * sum;
        }
        cache_size = new_size;
    }

    return cache[n];
}
