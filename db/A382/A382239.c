/*
 * A382239  Numbers not divisible by any of their digits nor by the sum of
 *          their digits. Digit 0 is allowed (and does not divide anything).
 * https://oeis.org/A382239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A382239:  Numbers not divisible by any of their digits nor by the sum of their digits. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Helper to test whether a number satisfies the definition. */
static inline bool is_valid(seq_t x)
{
    seq_t tmp = x;
    seq_t sum = 0;

    while (tmp > 0) {
        seq_t d = tmp % 10;
        tmp /= 10;
        if (d != 0 && x % d == 0) {
            return false;
        }
        sum += d;
    }
    if (sum != 0 && x % sum == 0) {
        return false;
    }
    return true;
}

/* The main function.  It lazily computes the sequence and caches
   previously found terms so that calls with arbitrary n are fast. */
seq_t a(idx_t n)
{
    /* Static cache state */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t candidate = 1;          /* next number to test */

    if (n == 0) {
        /* undefined for n<1 – return zero */
        return 0;
    }

    /* Grow the cache if necessary */
    while (cache_len < (size_t)n) {
        /* Find the next valid number */
        for (;;) {
            if (is_valid(candidate)) {
                /* Ensure capacity for the new term */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                    void *tmp = realloc(cache, new_cap * sizeof(seq_t));
                    cache = (seq_t *)tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = candidate;
                break;
            }
            candidate++;
        }
        candidate++;
    }

    return cache[(size_t)n - 1];
}
