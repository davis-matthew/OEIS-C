/*
 * A241788  Triangular numbers which have one or more occurrences of exactly
 *          five different digits.
 * https://oeis.org/A241788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   the nth triangular number whose decimal representation contains
 *            exactly five distinct digits
 * Range:     u8 none, u16 n <= 55, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 53 terms through n = 991 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Return true iff x has exactly five distinct decimal digits. */
static bool has_exactly_five_distinct_digits(seq_t x)
{
    unsigned mask = 0;
    while (x > (seq_t)0) {
        unsigned d = (unsigned)(x % (seq_t)10);
        mask |= 1u << d;
        x = x / (seq_t)10;
    }
    /* Count set bits in mask. */
    unsigned bits = 0;
    while (mask) {
        bits += mask & 1u;
        mask >>= 1;
    }
    return bits == 5;
}

/* Cache for found terms. */
static seq_t *cache     = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t search_k    = 0;

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Expand cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        ++search_k;
        /* t = k(k+1)/2 */
        seq_t t = (seq_t)search_k * ((seq_t)search_k + 1) / 2;
        if (has_exactly_five_distinct_digits(t)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) exit(EXIT_FAILURE);
                cache     = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = t;
        }
    }
    return cache[n - 1];
}
