/*
 * A393535  G.f. A(x) satisfies: A(x) = 1 + x * A(2*x/(1 - 2*x)) / (1 -
 *          2*x)^2.
 * https://oeis.org/A393535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = 2^(n-1) * Σ_{k=0}^{n-1} binomial(n,k+1) * a(k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   ensure_cache, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ensure_cache.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t max_computed = 0;   /* last computed index */

    /* ensure memory for [0..n] */
    ensure_cache(n, &cache, &cap);

    /* base term */
    if (n == 0)
        return 1;
    if (cache[0] == 0)
        cache[0] = 1;

    /* compute missing terms */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        seq_t binom = i;                 /* C(i,1) */

        for (idx_t k = 0; k < i; ++k) {
            seq_t prod = cache[k] * binom;
            sum += prod;

            if (k + 1 < i) {
                seq_t num = (seq_t)(i - (k + 1));
                seq_t den = (seq_t)(k + 2);
                binom = (binom * num) / den;    /* C(i,k+2) */
            }
        }

        cache[i] = sum * pow2(i - 1);
    }

    max_computed = n;
    return cache[n];
}
