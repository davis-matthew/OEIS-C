/*
 * A390559  Lower (1/2)-midsequence of (2^n) and F(n), where F = A000045
 *          (Fibonacci numbers); see Comments.
 * https://oeis.org/A390559
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-2) - a(n-3) - 3*a(n-4) + a(n-5) + 2*a(n-6)
 *            for n >= 7, with initial values: a(0)=0, a(1)=1, a(2)=2, a(3)=5,
 *            a(4)=9, a(5)=18, a(6)=36
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[7] = { 0, 1, 2, 5, 9, 18, 36 };
static seq_t *cache      = NULL;
static idx_t  cache_size = 0;
static idx_t  cached_cnt = 0;

static void ensure_cache_up_to(idx_t n) {
    /* Expand allocation if necessary */
    if (cache_size <= n) {
        idx_t new_size = cache_size == 0 ? 8 : cache_size;
        while (new_size <= n) new_size <<= 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) exit(1);          /* out of memory */
        cache = new_cache;
        if (cached_cnt == 0) {            /* first time: set known initials */
            for (idx_t i = 0; i < 7; ++i) cache[i] = init[i];
            cached_cnt = 7;
        }
        cache_size = new_size;
    }
    /* Compute new values up to n */
    if (cached_cnt <= n) {
        idx_t start = cached_cnt < 7 ? 7 : cached_cnt;
        for (idx_t i = start; i <= n; ++i) {
            seq_t val = 3 * cache[i-1] - cache[i-2]
                      - cache[i-3] - 3 * cache[i-4]
                      + cache[i-5] + 2 * cache[i-6];
            cache[i] = val;
        }
        cached_cnt = n + 1;
    }
}

seq_t a(idx_t n) {
    ensure_cache_up_to(n);
    return cache[n];
}
