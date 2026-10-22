/*
 * A398036  G.f. A(x) satisfies (1-3*x)*A(x) + A(x^4) = x.
 * https://oeis.org/A398036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n >= 2, a(n) = 3*a(n-1) - a(n/4) if 4 divides n,
 *            otherwise a(n) = 3*a(n-1)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;          /* 1‑based cache, seq_cache[0] unused */
static idx_t  cached_len = 0;           /* highest n in cache */
static size_t cache_cap = 0;            /* allocated size of seq_cache (>= cached_len + 1) */

seq_t a(idx_t n) {
    if (n == 0) return 0;               /* undefined, return 0 for safety */

    /* Initialise cache on first use */
    if (seq_cache == NULL) {
        seq_cache = (seq_t *)malloc(2 * sizeof(seq_t)); /* indices 0..1 */
        if (seq_cache == NULL) abort(); /* allocation failure */
        seq_cache[0] = 0;
        seq_cache[1] = 1;               /* a(1) */
        cached_len = 1;
        cache_cap = 2;
    }

    /* Grow the cache if required */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)(n + 1))
            new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        seq_cache = tmp;
        cache_cap = new_cap;
    }

    /* Build missing entries up to n */
    for (idx_t i = cached_len + 1; i <= n; ++i) {
        seq_t val = seq_cache[i - 1] * (seq_t)3;
        if ((i % 4) == 0) {
            /* i/4 is always < i */
            val -= seq_cache[i / 4];
        }
        seq_cache[i] = val;
    }
    cached_len = n;

    return seq_cache[n];
}
