/*
 * A368442  Expansion of e.g.f. exp(-x) / (3 - 2*exp(2*x)).
 * https://oeis.org/A368442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = (-1)^n + 2 * Sum_{k=1..n} 2^k * binomial(n,k) *
 *            a(n-k) for n>=1 The function memoises results for arbitrary
 *            order of calls
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;
static seq_t *pow2 = NULL;
static size_t pow2_len = 0, pow2_cap = 0;

seq_t a(idx_t n)
{
    /* ensure cache holds at least a[0] */
    if (cache_len == 0) {
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache[0] = 1;
        cache_len = 1;
    }
    /* ensure cache can hold up to n */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = (cache_cap < 4) ? 4 : cache_cap * 2;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
    /* ensure pow2 array covers at least n */
    if (pow2_len == 0) {
        pow2_cap = 1;
        pow2 = (seq_t *)malloc(sizeof(seq_t) * pow2_cap);
        pow2[0] = 1;
        pow2_len = 1;
    }
    if (pow2_cap <= (size_t)n) {
        size_t new_cap = (pow2_cap < 4) ? 4 : pow2_cap * 2;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(pow2, new_cap * sizeof(seq_t));
        pow2 = tmp;
        pow2_cap = new_cap;
    }
    /* fill pow2 up to n if needed */
    while (pow2_len <= (size_t)n) {
        pow2[pow2_len] = pow2[pow2_len - 1] * 2;
        ++pow2_len;
    }
    /* compute terms from current cache_len to n */
    size_t start = cache_len;
    for (size_t i = start; i <= (size_t)n; ++i) {
        seq_t sum = 0;
        seq_t binom = 1;
        for (size_t k = 1; k <= i; ++k) {
            binom = binom * (seq_t)(i - k + 1) / (seq_t)k; /* binomial(i,k) */
            seq_t term = binom;
            term = term * pow2[k];
            term = term * cache[i - k];
            sum += term;
        }
        seq_t val = sum * 2;
        if ((i & 1ULL) == 0) {
            val += 1;
        } else {
            val -= 1;
        }
        cache[i] = val;
    }
    cache_len = (size_t)n + 1;
    return cache[n];
}
