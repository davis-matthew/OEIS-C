/*
 * A327264  Numbers k with digit_sum(2*k) = digit_sum(2+k).
 * https://oeis.org/A327264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 3065, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: a(n) = the n-th positive integer k such that
   digit_sum(2*k) == digit_sum(2 + k) */

/* Include the provided helper for digit sums */
#include "../utils/digit_sum.h"
#include <stdlib.h>

/* static cache for memoization */
static seq_t *cache = NULL;      /* cached terms */
static idx_t  cache_len = 0;     /* number of cached terms */
static idx_t  cache_cap = 0;     /* allocated capacity of cache */
static seq_t  next_k   = 1;      /* next integer to test */

/* Allocate or grow the cache */
static void grow_cache(void) {
    if (cache_cap == 0) {
        cache_cap = 128;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
    } else {
        cache_cap *= 2;
        cache = (seq_t*)realloc(cache, sizeof(seq_t) * cache_cap);
    }
}

/* Generate terms until at least `up_to` terms are cached */
static void generate_until(idx_t up_to) {
    while (cache_len < up_to) {
        seq_t k = next_k;
        seq_t sum1 = digit_sum((seq_t)2 * k);   /* digit_sum(2*k)   */
        seq_t sum2 = digit_sum(k + (seq_t)2);   /* digit_sum(2+k)   */
        if (sum1 == sum2) {
            if (cache_len == cache_cap) grow_cache();
            cache[cache_len++] = k;
        }
        next_k++;
    }
}

/* Public function required by the problem statement */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* behavior below offset not required */
    generate_until(n);
    return cache[n - 1];
}
