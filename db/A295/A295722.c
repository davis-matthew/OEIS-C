/*
 * A295722  a(n) = a(n-1) + 3*a(n-2) -2*a(n-3) - 2*a(n-4), where a(0) = -1,
 *          a(1) = -1, a(2) = 2, a(3) = 3.
 * https://oeis.org/A295722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - 2*a(n-3) - 2*a(n-4) with a(0) = -1,
 *            a(1) = -1, a(2) = 2, a(3) = 3
 * Range:     i8 n <= 7, i16 n <= 18, i32 n <= 41, i64 n <= 87, i128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache      = NULL; /* cached sequence values   */
static size_t cache_cap  = 0;    /* allocated size of cache  */
static size_t computed   = 0;    /* number of terms already computed */

/* Ensure that cache can hold terms up to index n (inclusive). */
static void ensure_cache(size_t n)
{
    if (n < cache_cap) return;
    size_t newcap = cache_cap ? cache_cap << 1 : 16;
    while (newcap <= n) newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp) abort();          /* simple out‑of‑memory handling */
    cache    = tmp;
    cache_cap = newcap;
}

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;
    ensure_cache(idx);

    /* Compute terms sequentially up to idx if not already done. */
    while (computed <= idx) {
        if (computed == 0)
            cache[0] = (seq_t)-1;
        else if (computed == 1)
            cache[1] = (seq_t)-1;
        else if (computed == 2)
            cache[2] = (seq_t)2;
        else if (computed == 3)
            cache[3] = (seq_t)3;
        else {
            size_t i = computed;
            cache[i] =  cache[i-1]
                      + (seq_t)3 * cache[i-2]
                      - (seq_t)2 * cache[i-3]
                      - (seq_t)2 * cache[i-4];
        }
        ++computed;
    }

    return cache[idx];
}
