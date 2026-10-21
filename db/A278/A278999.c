/*
 * A278999  Numbers of the form (5h+2)*5^k-1 or (5h+3)*5^k-1 for h,k in N.
 * https://oeis.org/A278999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sequence of all positive integers n such that n+1 = (5h+2 or
 *            5h+3) * 5^k for some non‑negative integers h,k. Sequence is
 *            sorted in ascending order
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* static cache for memoization across calls */
static seq_t *seq_cache   = NULL;   /* dynamic array of computed terms */
static size_t seq_len     = 0;        /* number of terms stored */
static size_t seq_cap     = 0;        /* capacity of seq_cache */
static seq_t next_test    = 1;        /* next integer to test */

/* test whether a candidate number satisfies the definition */
static inline bool valid_value(seq_t x)
{
    seq_t y = x + 1;
    while (y % 5 == 0) y /= 5;
    return (y % 5 == 2) || (y % 5 == 3);
}

/* ensure seq_cache has at least 'needed' capacity */
static void ensure_capacity(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cache = new_arr;
    seq_cap   = new_cap;
}

/* public function as required */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1, but guard against 0 */

    size_t target = (size_t)n;
    /* generate terms until we have at least 'target' */
    while (seq_len < target) {
        /* find next valid number */
        while (true) {
            if (valid_value(next_test)) {
                ensure_capacity(seq_len + 1);
                seq_cache[seq_len++] = next_test;
                ++next_test;          /* advance to next candidate */
                break;
            }
            ++next_test;
        }
    }
    return seq_cache[n - 1];
}
