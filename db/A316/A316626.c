/*
 * A316626  a(1)=a(2)=a(3)=1; a(n) = a(n-2*a(n-1))+a(n-1-2*a(n-2)) for n > 3.
 * https://oeis.org/A316626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=a(2)=a(3)=1; a(n)=a(n-2*a(n-1))+a(n-1-2*a(n-2)) for n>3
 * Range:     u8 n <= 1012, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static idx_t max_cached = 0;

/* Expand the cache to include up to term n (1‑based). */
static void ensure_cache(idx_t n)
{
    if (n <= max_cached)
        return;
    size_t new_size = (size_t)(n + 1);          /* indices 0..n */
    seq_t *new_cache = NULL;
    if (cache == NULL) {
        new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    } else {
        new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    }
    if (new_cache == NULL)
        abort();                               /* out of memory */

    cache = new_cache;

    for (idx_t i = max_cached + 1; i <= n; ++i) {
        if (i <= 3) {
            cache[i] = (seq_t)1;
        } else {
            idx_t idx1 = i - 2 * (idx_t)(unsigned long long)cache[i - 1];
            idx_t idx2 = (i - 1) - 2 * (idx_t)(unsigned long long)cache[i - 2];
            cache[i] = cache[idx1] + cache[idx2];
        }
    }
    max_cached = n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                   /* undefined for n < 1 */
    ensure_cache(n);
    return cache[n];
}
