/*
 * A396474  Expansion of g.f. (1 - 7*x^2 - 2*x^4)/(1 - 2*x - 7*x^2 - 2*x^4 +
 *          2*x^5).
 * https://oeis.org/A396474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 7*a(n-2) + 2*a(n-4) - 2*a(n-5); a(0) = 1, a(1)
 *            = 2, a(2) = 4, a(3) = 22, a(4) = 72
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;       /* cache of sequence values */
static idx_t  cache_cap = 0;      /* allocated capacity */
static idx_t  cache_len = 0;      /* number of initialized values */

/* ensure at least `need+1` elements can be stored (indices 0..need) */
static void ensure_capacity(idx_t need)
{
    if (need < cache_cap)
        return;
    idx_t new_cap = cache_cap > 0 ? cache_cap : 8;
    while (new_cap <= need)
        new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Initialize cache on first use */
    if (!cache) {
        cache_cap = 8;
        cache_len = 5;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 2;
        cache[2] = 4;
        cache[3] = 22;
        cache[4] = 72;
    }
    if (n < cache_len)
        return cache[n];

    ensure_capacity(n);

    for (idx_t i = cache_len; i <= n; ++i) {
        /* recurrence: a(i) = 2*a(i-1) + 7*a(i-2) + 2*a(i-4) - 2*a(i-5) */
        seq_t val = 2 * cache[i - 1]
                  + 7 * cache[i - 2]
                  + 2 * cache[i - 4]
                  - 2 * cache[i - 5];
        cache[i] = val;
    }
    cache_len = n + 1;
    return cache[n];
}
