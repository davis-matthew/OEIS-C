/*
 * A365250  G.f. satisfies A(x) = 1 + x*A(x)^3/(1 - x^2*A(x)^6).
 * https://oeis.org/A365250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(3*n+1)) * Sum_{k=0..floor(n/2)} binomial(n-k-1,k) *
 *            binomial(3*n+1,n-2*k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* multiplicative binomial coefficient: C(n,k) with n,k >= 0 */
static seq_t binom_mul(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* extend cache up to n */
    if (n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!cache) return 0;  /* allocation failed */
        if (cache_len == 0) {
            cache[0] = 1;          /* a(0) = 1 */
            cache_len = 1;
        }
        /* compute missing terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t sum = 0;
            idx_t max_k = i / 2;
            for (idx_t k = 0; k <= max_k; ++k) {
                seq_t t1 = binom_mul(i - k - 1, k);
                seq_t t2 = binom_mul(3 * i + 1, i - 2 * k);
                sum += t1 * t2;
            }
            seq_t denom = (seq_t)(3 * i + 1);
            cache[i] = sum / denom;
        }
        cache_len = new_len;
    }
    return cache[n];
}
