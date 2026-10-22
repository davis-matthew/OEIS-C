/*
 * A366645  G.f. A(x) satisfies A(x) = 1 + x^3 * (A(x) / (1 - x))^4.
 * https://oeis.org/A366645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(n+k-1,n-3*k) *
 *            binomial(4*k,k) / (3*k+1)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 21, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n >= cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) {
            /* allocation failed – terminate */
            exit(EXIT_FAILURE);
        }
        cache = new_cache;

        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t sum = 0;

            if (i == 0) {
                sum = 1;                /* a(0) = 1 */
            } else {
                idx_t maxk = i / 3;
                for (idx_t k = 0; k <= maxk; ++k) {
                    seq_t bin1 = binom(i + k - 1, i - 3 * k);
                    seq_t bin2 = binom(4 * k, k);
                    seq_t div  = (seq_t)(3 * k + 1);
                    seq_t term;

                    term = bin1 * bin2;
                    term = term / div;
                    sum += term;
                }
            }
            cache[i] = sum;
        }
        cache_size = n + 1;
    }
    return cache[n];
}
