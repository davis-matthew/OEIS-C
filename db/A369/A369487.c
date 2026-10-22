/*
 * A369487  Expansion of (1/x) * Series_Reversion( x / (1-x) * (1-x-x^2)^3 ).
 * https://oeis.org/A369487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..floor(n/2)} binomial(3*n+k+2,k) *
 *            binomial(3*n-k+1,n-2*k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom.h"

static seq_t *cache = NULL;
static idx_t  cache_cap = 0;

/* Ensure the cache can hold terms up to index n. */
static void
ensure_cache(idx_t n)
{
    if (n < cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap <= n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                     /* unrecoverable allocation failure */
    for (idx_t i = cache_cap; i < new_cap; ++i)
        tmp[i] = (seq_t)0;            /* zero‑initialise new slots */
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute the single term a(n) from the defining sum. */
static seq_t
compute_term(idx_t n)
{
    if (n == 0)
        return (seq_t)1;              /* a(0)=1 */

    seq_t sum = (seq_t)0;
    idx_t max_k = n >> 1;              /* floor(n/2) */
    for (idx_t k = 0; k <= max_k; ++k)
    {
        seq_t b1 = binom(3 * n + k + 2, k);
        seq_t b2 = binom(3 * n - k + 1, n - 2 * k);
        sum += b1 * b2;
    }
    seq_t denom = (seq_t)(n + 1);       /* exact division by (n+1) */
    return sum / denom;
}

/* Public function required by the build system */
seq_t a(idx_t n)
{
    ensure_cache(n);
    if (cache[n] == (seq_t)0 && n != 0)
        cache[n] = compute_term(n);
    if (n == 0)                          /* store a(0) on first call */
        cache[0] = (seq_t)1;
    return cache[n];
}
