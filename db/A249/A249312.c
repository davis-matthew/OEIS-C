/*
 * A249312  Expansion of x*(1+11*x-10*x^3)/(1-12*x^2+10*x^4).
 * https://oeis.org/A249312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-2) - 10*a(n-4), with a(1)=1, a(2)=11, a(3)=12,
 *            a(4)=122
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 37, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;

static void ensure_cache_capacity(idx_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 1;
    while (new_cap < (size_t)needed) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort(); /* allocation failure */
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for n<1 */

    /* initialise first four terms once */
    if (cache_len == 0) {
        ensure_cache_capacity(4);
        cache[0] = 1;
        cache[1] = 11;
        cache[2] = 12;
        cache[3] = 122;
        cache_len = 4;
    }

    if (n <= cache_len) return cache[n-1];

    ensure_cache_capacity(n);
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        idx_t idx = i - 1; /* zero‑based index */
        /* recurrence: a(i) = 12*a(i-2) - 10*a(i-4) */
        seq_t val = (seq_t)12 * cache[idx-2] - (seq_t)10 * cache[idx-4];
        cache[idx] = val;
    }
    cache_len = n;
    return cache[n-1];
}
