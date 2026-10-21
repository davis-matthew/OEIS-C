/*
 * A290220  Narrow cross sequence (see Comments lines for definition).
 * https://oeis.org/A290220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4) for n > 9 a(0) = 0, a(1) = 2,
 *            a(2) = 6, a(3) = 10, a(4) = 18, a(5) = 26, a(6) = 34, a(7) = 42,
 *            a(8) = 58, a(9) = 70
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

seq_t a(idx_t n) {
    /* initialize cache on first call */
    if (cache == NULL) {
        size_t init_cap = 16;
        cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
        if (!cache) return 0;           /* out‑of‑memory; unlikely in tests */
        cache_cap = init_cap;
        cache_len = 10;
        static const seq_t init_vals[10] = {0, 2, 6, 10, 18, 26, 34, 42, 58, 70};
        for (size_t i = 0; i < 10; ++i)
            cache[i] = init_vals[i];
    }

    /* grow cache if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while ((size_t)n + 1 > new_cap)
            new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;           /* out‑of‑memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* return already computed value */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* compute values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        size_t ii = (size_t)i;
        if (i >= 10)                     /* indices <10 are pre‑filled */
            cache[ii] = cache[ii-1] + cache[ii-3] - cache[ii-4];
    }
    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}
