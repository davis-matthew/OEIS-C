/*
 * A330130  Number of length-n binary words having no even palindromes of
 *          length > 2 and no odd palindromes of length > 5.
 * https://oeis.org/A330130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-8) + a(n-10) for n >= 16 Initial values (a(0) to
 *            a(15)): 1, 2, 4, 8, 12, 18, 26, 28, 30, 32, 34, 36, 40, 44, 48,
 *            52
 * Range:     u8 n <= 35, u16 n <= 107, u32 n <= 250, u64 n <= 537,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static const seq_t init[16] = {
    1, 2, 4, 8, 12, 18, 26, 28,
    30, 32, 34, 36, 40, 44, 48, 52
};

/* Cache for already computed values */
static seq_t *cache      = NULL;
static idx_t  cache_len  = 0;        /* number of elements stored (cache_len-1 is last index) */
static size_t cache_cap  = 0;        /* allocated capacity */

/* Ensure the cache holds values up to index 'max_needed' (inclusive) */
static void ensure_cache(idx_t max_needed) {
    if (cache_len == 0) {
        /* Initial allocation and fill with first 16 values */
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return; /* out of memory, caller will get garbage */
        memcpy(cache, init, 16 * sizeof(seq_t));
        cache_len = 16;
    }

    if (max_needed < cache_len) return; /* already covered */

    /* Grow capacity as needed */
    size_t new_cap = cache_cap;
    while (new_cap <= (size_t)max_needed) {
        new_cap = new_cap ? new_cap * 2 : 16;
    }
    if (new_cap != cache_cap) {
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return; /* out of memory, caller will get garbage */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values */
    for (idx_t i = cache_len; i <= max_needed; ++i) {
        cache[i] = cache[i - 8] + cache[i - 10];
    }
    cache_len = max_needed + 1;
}

seq_t a(idx_t n) {
    if (n < 16) return init[n];
    ensure_cache(n);
    return cache[n];
}
