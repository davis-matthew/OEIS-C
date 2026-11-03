/*
 * A255341  Numbers n such that there is exactly one 1 in their factorial base
 *          representation (A007623).
 * https://oeis.org/A255341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 13069, u32 n <= 13069, u64 n <= 13069,
 *            u128 n <= 13069, big any n
 * Validated: standard, pass, 83 terms through n = 9449 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = n such that the factorial base representation of n contains exactly one digit 1 (A255341) */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Return true iff the factorial base representation of value has exactly one digit equal to 1. */
static bool has_exactly_one_one(seq_t value)
{
    idx_t k = 1;
    unsigned cnt = 0;
    while (value > 0) {
        seq_t mod = value % (seq_t)(k + 1);
        if (mod == 1) {
            ++cnt;
            if (cnt > 1) return false;
        }
        value /= (seq_t)(k + 1);
        ++k;
    }
    return cnt == 1;
}

/* Cache for previously computed terms */
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;
static idx_t test_start = 1;

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* Undefined for n < 1 */

    while (cache_len < n) {
        seq_t cand = (seq_t)test_start;
        if (has_exactly_one_one(cand)) {
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_len++] = cand;
        }
        ++test_start;
    }
    return cache[n - 1];
}
