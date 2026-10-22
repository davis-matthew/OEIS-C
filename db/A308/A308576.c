/*
 * A308576  a(n) = a(n-1) + a(a(n-1) mod n) + 1, a(0) = 1.
 * https://oeis.org/A308576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(a(n-1) mod n) + 1, a(0) = 1
 * Range:     u8 n <= 9, u16 n <= 37, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;

seq_t a(idx_t n) {
    /* Ensure the cache holds terms up to n */
    if (n == 0) {
        if (cache_size == 0) {
            if (cache_cap == 0) {
                cache_cap = 1;
                cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            } else {
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[0] = (seq_t)1;
            cache_size = 1;
        }
        return cache[0];
    }

    if (n >= cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failed; return some sentinel or abort */
            exit(EXIT_FAILURE);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        cache_size = 1;
    }

    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t mod  = prev % (seq_t)i;
        size_t mod_idx = (size_t)mod;
        seq_t term = prev + cache[mod_idx] + (seq_t)1;
        cache[i] = term;
    }
    cache_size = (size_t)n + 1;
    return cache[(size_t)n];
}
