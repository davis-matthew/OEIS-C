/*
 * A365246  G.f. satisfies A(x) = 1 + x*A(x)^2/(1 - x^2*A(x)^4).
 * https://oeis.org/A365246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(2*n+1)) * Sum_{k=0..floor(n/2)} binomial(n-k-1,k) *
 *            binomial(2*n+1,n-2*k)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Compute binomial coefficient nCk using an incremental product / division
   method that keeps intermediate values exact (works for any seq_t). */
static seq_t _binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;          /* use symmetry */

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);
        res = res / i;                 /* exact division */
    }
    return res;
}

static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;

seq_t a(idx_t n)
{
    /* Return cached value if already computed */
    if (n < cache_len) return cache[n];

    /* Ensure the cache is large enough */
    size_t need = (size_t)n + 1;
    if (need > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = need;
    }

    /* Compute all missing terms up to n, inclusive */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 1;
            continue;
        }

        idx_t limit = i / 2;
        seq_t sum = 0;
        for (idx_t k = 0; k <= limit; ++k) {
            seq_t t1 = _binom(i - k - 1, k);
            seq_t t2 = _binom(2 * i + 1, i - 2 * k);
            sum += t1 * t2;
        }
        seq_t denom = (seq_t)(2 * i + 1);
        cache[i] = sum / denom;        /* division is exact */
    }

    cache_len = (size_t)n + 1;
    return cache[n];
}
