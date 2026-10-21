/*
 * A257221  Numbers that have at least one divisor containing the digit 4 in
 *          base 10.
 * https://oeis.org/A257221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A257221.  
   The sequence consists of integers whose set of divisors contains at least
   one divisor with the decimal digit '4'.  The function generates terms by
   testing each integer in turn for such a divisor. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* helper: integer square root for uint64_t */
static inline uint64_t isqrt_u64(uint64_t n)
{
    if (n == 0) return 0;
    uint64_t x = n;
    while (1) {
        uint64_t y = (x + n / x) >> 1;
        if (y >= x) return x;
        x = y;
    }
}

/* helper: test whether a decimal number contains the digit 4 */
static inline bool contains_digit4(uint64_t v)
{
    while (v) {
        if (v % 10 == 4)
            return true;
        v /= 10;
    }
    return false;
}

/* dynamic cache for the sequence values */
static seq_t *seq_cache = NULL;
static size_t seq_len = 0;   /* number of terms stored */
static size_t seq_cap = 0;   /* capacity of the cache array */
static uint64_t checked_max = 0; /* largest integer examined so far */

/* append a value to the cache, resizing if necessary */
static void _append_cache(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 64;
        seq_t *new_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort(); /* allocation failed */
        seq_cache = new_cache;
        seq_cap = new_cap;
    }
    seq_cache[seq_len++] = val;
}

/* main function: return the n-th term of A257221 (1‑based index) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n = 0, harmless */

    /* fill cache until it has at least n terms */
    while (seq_len < (size_t)n) {
        ++checked_max;
        uint64_t k = checked_max;

        /* quick check: if k is a multiple of 4, divisor 4 exists */
        if (k % 4 == 0) {
            _append_cache((seq_t)k);
            continue;
        }

        bool has4 = false;
        uint64_t limit = isqrt_u64(k);
        for (uint64_t d = 1; d <= limit; ++d) {
            if (k % d == 0) {
                if (contains_digit4(d)) {
                    has4 = true;
                    break;
                }
                uint64_t d2 = k / d;
                if (d2 != d && contains_digit4(d2)) {
                    has4 = true;
                    break;
                }
            }
        }
        if (has4)
            _append_cache((seq_t)k);
    }

    return seq_cache[(size_t)n - 1];
}
