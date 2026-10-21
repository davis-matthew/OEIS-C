/*
 * A256385  Numbers n such that 2n^2+2n+1, 3n^2+6n+5, 6n^2+30n+55 are all
 *          composite.
 * https://oeis.org/A256385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 58, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A256385: numbers n such that
   2n^2+2n+1, 3n^2+6n+5, and 6n^2+30n+55 are all composite. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

/* static cache for already found terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t tested_up_to = 0;   /* last n that was checked */

/* Ensure cache can hold at least sz elements. */
static void ensure_cache_capacity(size_t sz) {
    if (sz <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < sz) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Test whether a 64‑bit value is composite. */
static inline bool composite(uint64_t v) {
    return v > 1 && !is_prime_small(v);
}

/* Generate the next qualifying n and store it in cache. */
static void generate_next(void) {
    for (;;) {
        tested_up_to++;                       /* try next integer */
        uint64_t n = (uint64_t)tested_up_to;  /* avoid overflow, same type */
        uint64_t t1 = 2 * n * n + 2 * n + 1;
        uint64_t t2 = 3 * n * n + 6 * n + 5;
        uint64_t t3 = 6 * n * n + 30 * n + 55;
        if (composite(t1) && composite(t2) && composite(t3)) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = (seq_t)n;
            break;
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* offset undefined, guard against misuse */
    while (cache_len < (size_t)n) {
        generate_next();
    }
    return cache[n - 1];
}
