/*
 * A375586  Expansion of e.g.f. 1 / (1 + x - x * exp(x^2/2)).
 * https://oeis.org/A375586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/2)} (n-2*k)! *
 *            Stirling2(k,n-2*k)/(2^k*k!)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t max_cached = 0;

    /* allocate and initialise cache if it does not exist yet */
    if (cache == NULL) {
        cache_cap = 3;                          /* need space for indices 0..2 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        max_cached = 2;
    }

    /* grow the cache to contain index n */
    if (n + 1 > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute missing a values on demand */
    if (n > max_cached) {
        for (idx_t idx = max_cached + 1; idx <= n; ++idx) {
            if (idx < 3) {
                /* already initialised for n = 0,1,2 */
                continue;
            }

            /* B corresponds to m = 1 initially:
               B = ((idx-2)*(idx-1)*idx)/2
            */
            seq_t B = (seq_t)(idx - 2);
            B = B * (seq_t)(idx - 1);
            B = B * (seq_t)idx;
            B = B / (seq_t)2;

            /* accumulate the sum for a[idx] */
            seq_t sum = B * cache[idx - 3];

            /* loop over m = 2 .. floor((idx-1)/2) */
            for (idx_t m = 2; 2 * m + 1 <= idx; ++m) {
                /* update B to the next term:
                   B = B * (idx - 2*m) * (idx - 2*m + 1) / (2*m)
                */
                B = B * (seq_t)(idx - 2 * m);
                B = B * (seq_t)(idx - 2 * m + 1);
                seq_t div = (seq_t)2 * m;
                B = B / div;

                /* add the new term to the sum */
                sum += B * cache[idx - (2 * m + 1)];
            }

            cache[idx] = sum;
        }
        max_cached = n;
    }

    return cache[n];
}
