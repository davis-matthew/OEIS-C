/*
 * A344375  Numbers n that can be written as the concatenation ab such that n
 *          mod (a*b) = a+b.
 * https://oeis.org/A344375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   This function computes the nth term of OEIS sequence A344375 by
 *            brute‑force search, testing each integer for the condition n mod
 *            (a*b) = a+b where n = concatenation of a and b. The found terms
 *            are cached
 * Range:     u8 n <= 48, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow_uint.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;
static seq_t next_candidate = 22;  /* last examined value; first number tested will be 23 */

/* Determine if x can be written as a valid concatenation ab satisfying the property. */
static inline bool is_valid(seq_t x) {
    seq_t tmp = x;
    size_t len = 0;
    while (tmp > 0) {
        tmp /= (seq_t)10;
        len++;
    }
    if (len < 2) return false;               /* need at least two digits */

    for (size_t d = 1; d < len; d++) {
        seq_t pow10_d  = pow_uint(10, d);     /* 10^d  */
        seq_t a = x / pow10_d;
        seq_t b = x % pow10_d;

        /* b must have exactly d digits (no leading zeros). */
        if (d > 1) {
            seq_t lower_bound = pow_uint(10, d - 1);
            if (b < lower_bound) continue;
        }
        if (b == 0) continue;                /* avoid division by zero */

        seq_t prod = a * b;
        if (prod == 0) continue;

        if (x % prod == a + b) return true;
    }
    return false;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined for index 0 */

    while (cache_size < n) {
        /* Find the next integer that satisfies the condition. */
        do {
            next_candidate += 1;
        } while (!is_valid(next_candidate));

        /* Resize cache if necessary. */
        if (cache_size == cache_cap) {
            size_t new_cap = cache_cap == 0 ? 64 : cache_cap * 2;
            seq_t *new_mem = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_mem) abort();  /* critical failure */
            cache = new_mem;
            cache_cap = new_cap;
        }
        cache[cache_size++] = next_candidate;
    }
    return cache[n - 1];
}
