/*
 * A308231  G.f.: x * Product_{k>=1} (1 + k^k*x^k)^(a(k)/k).
 * https://oeis.org/A308231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 8, u128 n <= 10,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1
   a(n+1) = (1/n) * Σ_{k=1..n} ( Σ_{d|k}(-1)^{k/d+1} * d^k * a(d) ) * a(n-k+1)   */

#include <stdlib.h>
#include "../utils/pow_uint.h"

static seq_t *cache_a = NULL;   /* stores a[1..] */
static seq_t *cache_S = NULL;   /* stores S[1..] where S(k)=Σ_{d|k}(-1)^{k/d+1} d^k a(d) */
static size_t cache_cap = 0;    /* capacity of cache_a (max index stored +1) */

/* Ensure cache_a has room for indices up to n (inclusive) */
static void ensure_cache_a(idx_t n)
{
    if (cache_cap >= (size_t)n + 1) return;
    size_t new_cap = (size_t)n + 1;
    seq_t *new_a = (seq_t *)realloc(cache_a, new_cap * sizeof(seq_t));
    seq_t *new_S = (seq_t *)realloc(cache_S, new_cap * sizeof(seq_t));
    if (!new_a || !new_S) return;                     /* unlikely in tests */
    for (size_t i = cache_cap; i < new_cap; ++i) {
        new_a[i] = (seq_t)0;
        new_S[i] = (seq_t)0;
    }
    cache_a = new_a;
    cache_S = new_S;
    cache_cap = new_cap;
}

/* Compute S(k) using already known a[ ] values */
static seq_t compute_S(idx_t k)
{
    seq_t sum_pos = (seq_t)0;
    seq_t sum_neg = (seq_t)0;
    for (idx_t d = 1; d * d <= k; ++d) {
        if (k % d != 0) continue;
        /* divisor d */
        idx_t other = k / d;
        int sign = ((other & 1) == 0) ? -1 : 1;          /* (-1)^{k/d+1} */
        seq_t term = pow_uint(d, k) * cache_a[d];
        if (sign > 0) sum_pos += term; else sum_neg += term;
        /* paired divisor other, if distinct */
        if (other != d) {
            int sign2 = ((d & 1) == 0) ? -1 : 1;
            seq_t term2 = pow_uint(other, k) * cache_a[other];
            if (sign2 > 0) sum_pos += term2; else sum_neg += term2;
        }
    }
    return sum_pos - sum_neg;
}

/* External function required by the build system */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n<1 */

    ensure_cache_a(n);

    /* Initialize base case a[1] if not done */
    if (cache_a[1] == (seq_t)0) {
        cache_a[1] = (seq_t)1;
        cache_S[1] = (seq_t)1;          /* S(1) = 1^1 * a(1) */
    }

    /* Compute terms up to n */
    for (idx_t idx = 2; idx <= n; ++idx) {
        if (cache_a[idx] != (seq_t)0) continue;    /* already computed */

        idx_t prev = idx - 1;
        if (cache_S[prev] == (seq_t)0)
            cache_S[prev] = compute_S(prev);

        seq_t numerator = (seq_t)0;
        for (idx_t k = 1; k <= prev; ++k) {
            numerator += cache_S[k] * cache_a[idx - k];
        }
        cache_a[idx] = numerator / (seq_t)(prev);
    }

    return cache_a[n];
}
