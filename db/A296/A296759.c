/*
 * A296759  Numbers whose base-16 digits d(m), d(m-1), ..., d(0) have #(rises)
 *          = #(falls); see Comments.
 * https://oeis.org/A296759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 9500, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A296759: Numbers whose base‑16 digits have equal number of rises and falls. */
/* Brute‑force search: test every integer starting at 1 and return the n-th that satisfies the condition. */

#include <stdbool.h>

/* Test whether a number has equal rises and falls in its hexadecimal representation. */
static bool good(seq_t x)
{
    seq_t t = x;
    unsigned prev = (unsigned)(t & (seq_t)0xFULL); /* least significant digit */
    t >>= 4;                 /* shift to next digit */
    unsigned rises = 0, falls = 0;

    while (t != 0) {
        unsigned cur = (unsigned)(t & (seq_t)0xFULL);
        if (prev < cur) rises++;
        else if (prev > cur) falls++;
        prev = cur;
        t >>= 4;
    }
    return rises == falls;
}

seq_t a(idx_t n)
{
    idx_t count = 1;
    seq_t val = 1;

    while (1) {
        if (good(val)) {
            if (count == n) return val;
            count++;
        }
        val++;
    }
}
