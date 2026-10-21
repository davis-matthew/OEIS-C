/*
 * A296551  Number of n X 5 0..1 arrays with each 1 horizontally, vertically
 *          or antidiagonally adjacent to 3 or 6 neighboring 1s.
 * https://oeis.org/A296551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-4) + a(n-6) + 3*a(n-7) + a(n-10) Initial
 *            terms: a(1)=1, a(2)=1, a(3)=4, a(4)=7, a(5)=11, a(6)=18,
 *            a(7)=34, a(8)=59, a(9)=100, a(10)=174
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 40, u64 n <= 80, u128 n <= 160,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Cache for computed terms. cache[0] is unused. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_last = 0;

/* Base values up to n = 10. */
static const seq_t base_vals[11] = {0,1,1,4,7,11,18,34,59,100,174};

static void ensure_cache(idx_t n) {
    if (n <= cache_last) return;
    size_t needed = (size_t)n + 1;          /* +1 because index 0 is unused */
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) exit(1);            /* Out of memory */
        if (cache) {
            for (size_t i = 0; i < cache_cap; ++i) new_cache[i] = cache[i];
            free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms. */
    for (idx_t k = cache_last + 1; k <= n; ++k) {
        if (k <= 10) {
            cache[k] = base_vals[(size_t)k];
        } else {
            seq_t val = cache[k-1];
            val += 3 * cache[k-4];
            val += cache[k-6];
            val += 3 * cache[k-7];
            val += cache[k-10];
            cache[k] = val;
        }
    }
    cache_last = n;
}

/* External function required by the build. */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for n<1, return 0 as a safe value */
    ensure_cache(n);
    return cache[(size_t)n];
}
