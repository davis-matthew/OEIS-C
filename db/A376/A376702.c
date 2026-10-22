/*
 * A376702  Numbers k whose nonzero digits are strictly decreasing when
 *          written in factoradic.
 * https://oeis.org/A376702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 1430, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   good_factoradic
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k whose nonzero factoradic digits are strictly decreasing */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/good_factoradic.h"

static seq_t *seq_cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_candidate = 1;

static void grow_cache(size_t needed_cap) {
    if (cache_cap >= needed_cap)
        return;
    size_t new_cap = cache_cap == 0 ? 8 : cache_cap;
    while (new_cap < needed_cap)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;
    while (cache_len < (size_t)n) {
        if (good_factoradic(next_candidate)) {
            grow_cache(cache_len + 1);
            seq_cache[cache_len++] = (seq_t)next_candidate;
        }
        next_candidate++;
    }
    return seq_cache[(size_t)n - 1];
}
