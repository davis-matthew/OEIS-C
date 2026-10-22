/*
 * A329256  Expansion of e.g.f. exp(Sum_{k>=1} x^(k^2) / (k^2)!).
 * https://oeis.org/A329256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = Σ_{k=1..n} binomial(n-1, k-1) * [k is a perfect
 *            square] * a(n-k) Where [k is a perfect square] is 1 if k is a
 *            square else 0. This recurrence follows from the e.g.f. expansion
 *            exp( Σ_{t≥1} x^(t^2)/(t^2)! )
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 19, u64 n <= 30, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Minimal multiplicative binomial coefficient.
   The division is exact for integer binomial coefficients. */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k == 0 || k == n) return (seq_t)1;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

/* Cache for already computed terms. */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* Ensure cache contains values up to at least index n. */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;

    idx_t new_size = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) exit(1);           /* abort on allocation failure */
    cache = new_cache;

    /* Initialize newly added entries. */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;       /* a(0) = 1 */
            continue;
        }
        seq_t sum = (seq_t)0;
        /* loop over squares k = s^2 <= i */
        for (idx_t s = 1; ; ++s) {
            idx_t k = s * s;
            if (k > i) break;
            seq_t bin = binom(i - 1, k - 1);   /* binomial(i-1, k-1) */
            seq_t val = cache[i - k];
            sum = sum + bin * val;
        }
        cache[i] = sum;
    }

    cache_size = new_size;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
