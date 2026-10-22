/*
 * A346889  Expansion of e.g.f. 1 / (1 - x^3 * exp(x) / 3!).
 * https://oeis.org/A346889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n)=∑_{k=3..n} binomial(n,k)*binomial(k,3)*a(n-k) ; a(0)=1
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*         a(1)=0, a(2)=0 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* number of terms stored (cache[0..cache_cap-1]) */

    /* initialise cache with a(0)=1 */
    if (cache_cap == 0) {
        cache_cap = 1;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 1;
    }

    /* extend cache up to n if necessary */
    if (n >= cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));

        for (idx_t i = cache_cap; i <= n; ++i) {
            seq_t sum = 0;
            seq_t binom_val = 1;       /* binomial(i,0) */

            for (idx_t k = 1; k <= i; ++k) {
                /* binomial(i,k) = binomial(i,k-1)*(i-k+1)/k  */
                binom_val = binom_val * (seq_t)(i - k + 1) / (seq_t)k;

                if (k >= 3) {
                    seq_t tri = (seq_t)(k * (k - 1) * (k - 2) / 6);
                    seq_t term = binom_val * tri * cache[i - k];
                    sum += term;
                }
            }
            cache[i] = sum;
        }
        cache_cap = (idx_t)new_cap;
    }

    return cache[n];
}
