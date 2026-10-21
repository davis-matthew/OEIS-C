/*
 * A296572  Number of n X 2 0..1 arrays with each 1 horizontally, vertically
 *          or antidiagonally adjacent to 0, 1 or 3 neighboring 1s.
 * https://oeis.org/A296572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 4*a(n-2) + 3*a(n-3) + a(n-4) + 4*a(n-5) - a(n-6)
 *            - 6*a(n-7) - a(n-8) + a(n-9) Initial values: a(1) = 4, a(2) =
 *            11, a(3) = 31, a(4) = 90, a(5) = 254, a(6) = 728, a(7) = 2078,
 *            a(8) = 5931, a(9) = 16941
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* cache[1] holds a(1) */
static size_t cache_len = 0;    /* highest index computed */
static size_t cache_cap = 0;    /* allocated capacity */

/* Ensure the cache has room for indices up to 'needed' (inclusive) */
static void ensure_cache_size(size_t needed)
{
    if (cache_cap < needed + 1) {          /* +1 because we use 1‑based indices */
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < needed + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* behaviour below the offset is undefined */

    size_t idx = (size_t)n;  /* n is guaranteed to fit into size_t for our use */

    /* Initialise the cache with the first 9 terms on the first call. */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;  /* allocation failure, give 0 as a safe fall‑back */

        cache[1] = (seq_t)4;
        cache[2] = (seq_t)11;
        cache[3] = (seq_t)31;
        cache[4] = (seq_t)90;
        cache[5] = (seq_t)254;
        cache[6] = (seq_t)728;
        cache[7] = (seq_t)2078;
        cache[8] = (seq_t)5931;
        cache[9] = (seq_t)16941;
        cache_len = 9;
    }

    /* If the requested value is already cached, return it. */
    if (idx <= cache_len) return cache[idx];

    /* Expand cache to hold the new indices. */
    ensure_cache_size(idx);

    /* Compute terms from cache_len+1 up to idx using the recurrence. */
    for (size_t i = cache_len + 1; i <= idx; ++i) {
        seq_t val = cache[i - 1];
        val += (seq_t)4 * cache[i - 2];
        val += (seq_t)3 * cache[i - 3];
        val +=       cache[i - 4];
        val += (seq_t)4 * cache[i - 5];
        val -=       cache[i - 6];
        val -= (seq_t)6 * cache[i - 7];
        val -=       cache[i - 8];
        val +=       cache[i - 9];
        cache[i] = val;
    }

    cache_len = idx;
    return cache[idx];
}
