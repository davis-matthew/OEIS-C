/*
 * A385765  G.f. A(x) satisfies A(x) = 1/(1 - x*A(x) - x^6*A'''''(x)).
 * https://oeis.org/A385765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1
   a(n) = sum_{k=0..n-1} (1 + 24*k - 50*k^2 + 35*k^3 - 10*k^4 + k^5) * a(k) * a(n-1-k)   */

#include <stdlib.h>

#define A385765_MAX 10000          /* enough for all n required in the tests */

static seq_t cache[A385765_MAX + 1] = {0};
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    /* Initialise the cache on the first call */
    if (cache_len == 0) {
        cache[0] = 1;
        cache_len = 1;
    }

    /* Return already computed values */
    if (n < cache_len)
        return cache[n];

    /* Compute terms up to n (must be <= A385765_MAX) */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 0; k < i; ++k) {
            seq_t kk = (seq_t)k;
            seq_t w = 1;                 /* 1 + 24k - 50k^2 + 35k^3 - 10k^4 + k^5 */
            w += 24 * kk;                /* +24k */
            w += 35 * kk * kk * kk;      /* +35k^3 */
            w += kk * kk * kk * kk * kk; /* +k^5  */
            w -= 10 * kk * kk * kk * kk; /* -10k^4 */
            w -= 50 * kk * kk;           /* -50k^2 */

            seq_t prod = cache[k] * cache[i - 1 - k];
            prod *= w;
            sum += prod;
        }
        cache[i] = sum;
    }
    cache_len = n + 1;
    return cache[n];
}
