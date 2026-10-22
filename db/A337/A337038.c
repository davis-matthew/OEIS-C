/*
 * A337038  a(n) = exp(-1/2) * Sum_{k>=0} (2*k - 1)^n / (2^k * k!).
 * https://oeis.org/A337038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A337038
   Recurrence: a(0) = 1;
                a(n) = ∑_{k=1}^{n-1} C(n-1,k) * 2^k * a(n-k-1)   (n ≥ 1)
*/

#include <stddef.h>   /* for NULL, size_t */
#include <stdlib.h>   /* for malloc, realloc, abort */

static seq_t *cache = NULL;      /* Cached values a[0] … a[cache_len-1] */
static idx_t  cache_len = 0;     /* Number of cached terms */

/* Ensure cache has room for indices 0 .. n inclusive */
static void ensure_cache_size(idx_t n)
{
    if (cache_len > n)
        return;
    size_t new_len = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache)
        abort();   /* allocation failure */
    cache = new_cache;
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    /* Remember current cache size before possible reallocation */
    idx_t start = cache_len;
    ensure_cache_size(n);

    for (idx_t i = start; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
            continue;
        }
        seq_t sum   = 0;                /* a(i) */
        seq_t binom = 1;                /* C(i-1,0) */
        seq_t pow2  = 2;                /* 2^1 initially */
        idx_t N = i - 1;                /* upper bound for k */

        for (idx_t k = 1; k <= N; ++k) {
            /* Update binomial coefficient: C(N,k) = C(N,k-1) * (N-k+1)/k */
            binom = binom * (seq_t)(N - (k - 1)) / (seq_t)k;
            seq_t term = binom * pow2 * cache[i - k - 1];
            sum = sum + term;
            pow2 = pow2 * (seq_t)2;     /* next power of two */
        }
        cache[i] = sum;
    }

    return cache[n];
}
