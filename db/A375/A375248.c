/*
 * A375248  Expansion of (1 - x)/(1 - 2*x - 3*x^2)^(7/2).
 * https://oeis.org/A375248
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n+5)/(n*(n+4))) * ((2*n+3)*a(n-1) + 3*(n+4)*a(n-2))
 *            with a(0) = 1, a(1) = 6
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t  cached_end = (idx_t)-1;
static size_t cache_cap   = 0;

/* Ensure that cache[0..n] is filled */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* initial values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
        cached_end = 1;
    }
    if (n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (n >= new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return; /* allocation failure – not expected */
        cache = tmp;
        cache_cap = new_cap;
    }
    if (n <= cached_end) return;

    for (idx_t i = cached_end + 1; i <= n; ++i) {
        seq_t ii           = (seq_t)i;
        seq_t ii_plus_4    = ii + 4;
        seq_t ii_plus_5    = ii + 5;
        seq_t two_ii_plus_3 = ii * 2 + 3;

        seq_t prod1 = two_ii_plus_3 * cache[i - 1];
        seq_t prod2 = (seq_t)3 * ii_plus_4 * cache[i - 2];
        seq_t A = prod1 + prod2;

        seq_t numerator = ii_plus_5 * A;
        seq_t denom     = ii * ii_plus_4;
        cache[i] = numerator / denom;
    }
    cached_end = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;
    ensure_cache(n);
    return cache[n];
}
