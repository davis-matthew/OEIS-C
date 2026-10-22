/*
 * A300632  Expansion of e.g.f. exp(x + Sum_{k>=2} prime(k-1)*x^k/k!).
 * https://oeis.org/A300632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} binom(n-1, k-1) * b(k) * a(n-k) where b(1)=1
 *            and for k≥2, b(k)=prime(k-1) (A008578) a(0)=1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 37,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"  /* provides uint32_t get_prime(idx_t k) */
#include <stddef.h>
#include <stdlib.h>

/* ---- cache storage ---------------------------------------------------- */
static seq_t *cache = NULL;    /* array of a(n) values */
static size_t cache_cap = 0;   /* number of allocated slots */
static size_t cache_len = 0;   /* number of computed slots (0..cache_len-1) */

/* Ensure the cache can hold indices up to n (inclusive). */
static void grow_cache(idx_t n)
{
    if ((size_t)n < cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp) {
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* ---- main sequence function ------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) {            /* initial value */
        if (cache_len == 0) {
            grow_cache(0);
            cache[0] = (seq_t)1;
            cache_len = 1;
        }
        return cache[0];
    }

    if ((size_t)n < cache_len)      /* value already cached */
        return cache[(size_t)n];

    grow_cache(n);
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        seq_t sum = cache[(size_t)(i - 1)];  /* k = 1 term: b(1)=1 */

        seq_t bin = (seq_t)1;                /* binom(i-1, 0) = 1 */
        for (idx_t k = 2; k <= i; ++k) {
            bin = bin * (seq_t)(i - k + 1) / (seq_t)(k - 1);

            uint32_t p = get_prime(k - 1);      /* b(k) = prime(k-1) */
            seq_t term = (seq_t)p * bin * cache[(size_t)(i - k)];
            sum += term;
        }
        cache[(size_t)i] = sum;
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
