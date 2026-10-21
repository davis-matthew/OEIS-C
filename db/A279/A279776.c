/*
 * A279776  Numbers n such that the sum of digits of 6n equals 12.
 * https://oeis.org/A279776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   Return the nth integer n such that the sum of digits of 6*n
 *            equals 12
 * Range:     u8 n <= 9, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t next_candidate = 1;
static idx_t found_count = 0;

static void ensure_cap(size_t needed) {
    if (cache_cap < needed) {
        size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
        while (new_cap < needed) new_cap *= 2;
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_cache;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* offset is 1; behaviour for n==0 is irrelevant */

    while (found_count < n) {
        seq_t prod = (seq_t)next_candidate * 6;
        if (digit_sum(prod) == 12) {
            ensure_cap(found_count + 1);
            cache[found_count] = (seq_t)next_candidate;
            found_count++;
        }
        next_candidate++;
    }
    return cache[n - 1];
}
