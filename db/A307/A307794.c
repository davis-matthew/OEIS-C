/*
 * A307794  a(1) = 1; a(n+1) = Sum_{d|n} sigma(d)*a(d), where sigma = sum of
 *          divisors (A000203).
 * https://oeis.org/A307794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; a(n+1)=Σ_{d|n} σ(d)·a(d)  (σ = sum of divisors) */
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static seq_t *sigma = NULL;
static idx_t sigma_size = 0;

/* floor integer square root */
static inline idx_t isqrt(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= n)
        ++r;
    return r;
}

/* recompute sigma[1..N] */
static void recompute_sigma(idx_t N)
{
    seq_t *new_sigma = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!new_sigma) exit(1);
    for (idx_t i = 0; i <= N; ++i) new_sigma[i] = 0;

    for (idx_t i = 1; i <= N; ++i) {
        for (idx_t j = i; j <= N; j += i) {
            new_sigma[j] += (seq_t)i;
        }
    }
    if (sigma) free(sigma);
    sigma = new_sigma;
    sigma_size = N;
}

/* sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, return 0 */

    if (n > cache_size) {
        /* extend sigma if needed */
        if (n > sigma_size)
            recompute_sigma(n);

        /* allocate new cache array */
        seq_t *new_cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!new_cache) exit(1);
        for (idx_t i = 0; i <= n; ++i) new_cache[i] = 0;

        /* copy existing values */
        for (idx_t i = 1; i <= cache_size; ++i) new_cache[i] = cache[i];
        if (cache) free(cache);
        cache = new_cache;

        /* compute new terms */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i == 1) {
                cache[1] = (seq_t)1;
            } else {
                idx_t m = i - 1;
                seq_t sum = 0;
                idx_t sq = isqrt(m);
                for (idx_t d = 1; d <= sq; ++d) {
                    if (m % d == 0) {
                        sum += sigma[d] * cache[d];
                        idx_t d2 = m / d;
                        if (d2 != d)
                            sum += sigma[d2] * cache[d2];
                    }
                }
                cache[i] = sum;
            }
        }
        cache_size = n;
    }
    return cache[n];
}
