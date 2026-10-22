/*
 * A364199  Expansion of e.g.f. 2*x/(exp(-2*x)+exp(x)).
 * https://oeis.org/A364199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=0, a(1)=1, for n≥2: a(n) = -½ * Σ_{k=0}^{n-1} a(k) *
 *            ((-2)^{n-k} + 1) * C(n,k)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;    /* memoised values a(0),a(1),… */
    static idx_t  cache_len = 0;   /* number of values stored  */
    static size_t cache_cap = 0;   /* allocated capacity            */

    /* ensure storage covers indices up to n */
    if (cache_cap < (size_t)(n + 1)) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise the first two values if necessary */
    if (cache_len == 0) {
        cache[0] = 0;
        cache_len = 1;
    }
    if (n >= 1 && cache_len <= 1) {
        cache[1] = 1;
        cache_len = 2;
    }
    /* quick return for n=0 or when the value is already computed */
    if (n < cache_len)
        return cache[n];

    /* compute missing terms up to n */
    for (idx_t k = cache_len; k <= n; ++k) {
        if (k == 0) {
            cache[0] = 0;
            continue;
        }
        if (k == 1) {
            cache[1] = 1;
            continue;
        }

        /* compute Σ_{i=0}^{k-1} a(i)*(((-2)^{k-i}+1)*C(k,i)) */
        seq_t sum = 0;

        /* find (-2)^k */
        seq_t pow_neg2 = 1;
        for (idx_t t = 0; t < k; ++t)
            pow_neg2 *= -2;

        /* binomial coefficient C(k,i); start with C(k,0)=1 */
        seq_t binom = 1;

        for (idx_t i = 0; i < k; ++i) {
            sum += cache[i] * (pow_neg2 + 1) * binom;

            /* update binomial for next i: C(k,i+1) = C(k,i)*(k-i)/(i+1) */
            if (i + 1 < k) {
                binom = binom * (k - i) / (i + 1);
                pow_neg2 /= -2;          /* (-2)^{k-(i+1)} = (-2)^{k-i} / (-2) */
            }
        }

        cache[k] = -sum / 2;
        cache_len = k + 1;
    }

    return cache[n];
}
