/*
 * A244451  E.g.f.: exp( Sum_{n>=1} Fibonacci(2*n)*x^n/n ).
 * https://oeis.org/A244451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n - 2)*a(n-1) - (n - 1)*(n - 2)*a(n-2), a(0)=a(1)=1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Cache of previously computed terms.  */
static seq_t *cache = NULL;
static size_t  cache_len  = 0;
static idx_t   cache_max  = (idx_t)-1;   /* maximum n computed so far */

/* Ensure that cache contains all terms up to and including n. */
static void ensure_cache(idx_t n)
{
    /* initialise if not yet done */
    if (cache_len == 0) {
        cache_len = 2;
        if (n + 1 > cache_len) {
            while (cache_len <= n + 1) cache_len <<= 1;
        }
        cache = (seq_t*)calloc(cache_len, sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache_max = 1;
    }
    /* already have it? */
    if (n <= cache_max) return;

    /* grow array if needed */
    if ((size_t)n + 1 > cache_len) {
        size_t new_len = cache_len;
        while (new_len <= (size_t)n + 1) new_len <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();
        cache     = tmp;
        cache_len = new_len;
    }

    /* compute new terms */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t coeff1 = (seq_t)((3ULL * i) - 2ULL);          /* (3*i-2) */
        seq_t term1  = coeff1 * cache[i - 1];

        seq_t coeff2 = ((seq_t)(i - 1)) * ((seq_t)(i - 2));   /* (i-1)(i-2) */
        seq_t term2  = coeff2 * cache[i - 2];

        cache[i] = term1 - term2;
    }
    cache_max = n;
}

/* External function that returns a(n). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    ensure_cache(n);
    return cache[n];
}
