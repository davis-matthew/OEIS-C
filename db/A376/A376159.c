/*
 * A376159  G.f. satisfies A(x) = 1 / ((1-x)^3 - x*A(x)).
 * https://oeis.org/A376159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A376159:  a(0)=1,  
   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) 
          + sum_{i=0}^{n-1} a[i]*a[n-1-i]         (n ≥ 1)
   The implementation computes terms incrementally and memoises
   results so that subsequent calls with larger or arbitrary n
   require only the missing terms to be computed.
*/
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoised terms */
    static idx_t cache_size = 0;  /* number of terms stored */

    /* initialise: a(0) = 1 */
    if (cache == NULL) {
        cache_size = 1;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;           /* allocation failure */
        cache[0] = (seq_t)1;
    }

    /* extend cache up to n if necessary */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failure */
        cache = tmp;

        seq_t mul3 = (seq_t)3;

        for (idx_t i = cache_size; i <= n; ++i) {
            /* convolution sum: sum_{k=0}^{i-1} a[k] * a[i-1-k] */
            seq_t sum = (seq_t)0;
            idx_t lim = i - 1;
            for (idx_t j = 0; j <= lim / 2; ++j) {
                seq_t prod = cache[j] * cache[lim - j];
                if (j != lim - j)
                    sum += prod + prod;   /* double for symmetric pairs */
                else
                    sum += prod;
            }

            seq_t val = sum;
            if (i >= 1) val += cache[i-1] * mul3;
            if (i >= 2) val -= cache[i-2] * mul3;
            if (i >= 3) val += cache[i-3];

            cache[i] = val;
        }
        cache_size = new_size;
    }

    return cache[n];
}
