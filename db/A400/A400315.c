/*
 * A400315  G.f. A(x) satisfies A(x) = A(x^4/(1 - x)) / (1 - x)^2, with A(0) =
 *          1.
 * https://oeis.org/A400315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(n)=∑_{k=0}^{⌊n/4⌋} binomial(n-3k+1,k+1)*a(k)
 * Range:     u8 n <= 12, u16 n <= 26, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k); */

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;      /* number of stored terms (maximum index+1)    */
    static size_t cache_cap = 0;     /* allocated capacity                         */

    /* Initialise cache to hold at least a(0) */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;   /* a(0) = 1 */
        cache_len = 1;
    }

    /* Expand cache to include n if necessary */
    if (n + 1 > cache_len) {
        /* Ensure capacity: double until sufficient */
        if (cache_cap < ((size_t)n + 1)) {
            size_t new_cap = cache_cap;
            while (new_cap < ((size_t)n + 1)) new_cap <<= 1;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        /* Compute terms from current length up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            idx_t limit = i / 4;
            for (idx_t k = 0; k <= limit; ++k) {
                idx_t nn = i - 3 * k + 1;
                idx_t kk = k + 1;
                seq_t bin = binomial(nn, kk);
                sum += bin * cache[k];
            }
            cache[i] = sum;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
