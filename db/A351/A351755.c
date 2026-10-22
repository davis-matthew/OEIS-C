/*
 * A351755  G.f. A(x) satisfies: A(x) = 1 + x + x^2 + x^3 + x^4 + x^5 + x^6 *
 *          A(x/(1 - x)) / (1 - x)^2.
 * https://oeis.org/A351755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1, a(2)=1, a(3)=1, a(4)=1, a(5)=1, a(n) =
 *            Σ_{k=0}^{n-6} binomial(n-5, k+1) * a(k) for n ≥ 6
 * Range:     u8 n <= 12, u16 n <= 18, u32 n <= 28, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


/* memoisation storage */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    /* grow cache and compute missing entries */
    if (cache_len <= n) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);
        cache = new_cache;

        for (idx_t i = cache_len; i <= n; ++i) {
            if (i < 6) {
                cache[i] = (seq_t)1;
            } else {
                seq_t sum = 0;
                idx_t limit = i - 6;          /* k ranges 0 .. i-6 */
                idx_t n_val = i - 5;          /* n in binomial(n, r) */
                seq_t bin = (seq_t)n_val;     /* binomial(n,1) */

                for (idx_t k = 0; k <= limit; ++k) {
                    sum += cache[k] * bin;
                    if (k < limit) {
                        /* next binomial: bin = bin * (n - (k+1)) / (k+2) */
                        bin = bin * (seq_t)(n_val - (k + 1));
                        bin = bin / (seq_t)(k + 2);
                    }
                }
                cache[i] = sum;
            }
        }
        cache_len = n + 1;
    }
    return cache[n];
}
