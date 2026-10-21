/*
 * A268902  Number of n X 6 0..2 arrays with some element plus some
 *          horizontally or antidiagonally adjacent neighbor totalling two
 *          exactly once.
 * https://oeis.org/A268902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 50*a(n-1) - 805*a(n-2) + 4662*a(n-3) - 12150*a(n-4) +
 *            14580*a(n-5) - 6561*a(n-6) with initial values a(1)=240,
 *            a(2)=9720, a(3)=299088, a(4)=8250912, a(5)=214142760,
 *            a(6)=5344944120
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Store the initial values once */
static void init_cache(idx_t n)
{
    if (cache_cap < 6) {
        size_t new_cap = (n > 6 ? n : 6);
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();  /* memory failure */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[0] = (seq_t)240;
    cache[1] = (seq_t)9720;
    cache[2] = (seq_t)299088;
    cache[3] = (seq_t)8250912;
    cache[4] = (seq_t)214142760;
    cache[5] = (seq_t)5344944120;
    cache_len = 6;
}

/* Ensure the cache can hold at least n entries and compute missing ones */
static void ensure_cache(idx_t n)
{
    if (cache_len >= n) return;
    if (cache_cap < n) {
        size_t new_cap = n;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();  /* memory failure */
        cache = tmp;
        cache_cap = new_cap;
    }
    if (cache_len < 6) init_cache(n);
    for (size_t i = cache_len; i < n; ++i) {
        idx_t k = i;                     /* 0‑based index for a(k+1) */
        seq_t t = (seq_t)50  * cache[k-1]
                - (seq_t)805  * cache[k-2]
                + (seq_t)4662 * cache[k-3]
                - (seq_t)12150 * cache[k-4]
                + (seq_t)14580 * cache[k-5]
                - (seq_t)6561 * cache[k-6];
        cache[k] = t;
    }
    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0, but harmless */
    ensure_cache(n);
    return cache[n-1];
}
