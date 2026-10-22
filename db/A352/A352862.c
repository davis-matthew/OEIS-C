/*
 * A352862  a(n) = 1 + Sum_{k=0..n-1} binomial(n+3,k+4) * a(k).
 * https://oeis.org/A352862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n>0, * a(n) = 1 + Sum_{k=0}^{n-1} binomial(n+3,
 *            k+4) * a(k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 22, u128 n <= 23,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a simple memoised iterative implementation. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "../utils/binomial.h"

static seq_t *cache = NULL;          /* holds computed a(n) values   */
static size_t cache_cap = 0;         /* current capacity              */
static idx_t cached_max = (idx_t)-1; /* highest n that has been cached */

/* Ensure the cache is large enough to hold indices up to n (inclusive).  */
static void ensure_cache_cap(idx_t n)
{
    if ((size_t)n < cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while ((size_t)n >= new_cap)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                      /* no dynamic memory left */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* initialise cache if needed */
    if (cached_max == (idx_t)-1) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache)
            abort();
        cache_cap = 1;
        cache[0] = (seq_t)1;
        cached_max = 0;
    }

    /* extend cache capacity if required */
    ensure_cache_cap(n);

    /* compute any missing values up to n */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        if (i == 0) {                  /* already set, keep for safety */
            cache[0] = (seq_t)1;
            continue;
        }
        seq_t sum = (seq_t)0;
        idx_t r_start = 4;
        idx_t r_end = i + 3;

        /* start with coeff = binomial(i+3,4) */
        seq_t coeff = binomial((idx_t)i + 3, (idx_t)4);

        for (idx_t r = r_start, k = 0; r <= r_end; ++k, ++r) {
            sum += coeff * cache[k];
            if (r < r_end) {
                /* coeff = coeff * (i+3 - r) / (r+1) */
                seq_t num = coeff * (seq_t)((seq_t)i + 3 - (seq_t)r);
                seq_t den = (seq_t)(r + 1);
                coeff = num / den;
            }
        }
        cache[i] = sum + (seq_t)1;
    }
    cached_max = n;
    return cache[n];
}
