/*
 * A352649  Expansion of e.g.f. 1/(1 - 3 * x * cosh(x)).
 * https://oeis.org/A352649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = 3 * Σ_{k=0..⌊(n-1)/2⌋} (2k+1) * C(n,2k+1) *
 *            a(n-2k-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* multiplicative binomial C(n,k) */
static seq_t binom_mul(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * ((seq_t)(n - k + i));
        res = res / ((seq_t)i);   /* exact division */
    }
    return res;
}

/* ensure cache array large enough for index n */
static void ensure_cache_up_to(idx_t n)
{
    if ((idx_t)cache_cap > n) return;
    size_t new_cap = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();
    /* initialize new slots to 0 */
    for (size_t i = cache_cap; i < new_cap; ++i) new_cache[i] = 0;
    /* set the first term if this is the first allocation */
    if (cache == NULL) new_cache[0] = (seq_t)1;
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    ensure_cache_up_to(n);
    if (cache[n] != 0) return cache[n];   /* already computed */

    seq_t sum = 0;
    idx_t maxk = (n - 1) / 2;
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t odd = 2 * k + 1;
        seq_t bin = binom_mul(n, odd);
        seq_t term = bin * cache[n - odd];
        term = term * (seq_t)odd;
        sum += term;
    }
    sum *= 3;
    cache[n] = sum;
    return sum;
}
