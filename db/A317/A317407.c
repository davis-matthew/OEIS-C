/*
 * A317407  The "OOPS" numbers -- numbers with ones in all odd-numbered
 *          positions of the binary representation of n.
 * https://oeis.org/A317407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 765, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A317407: numbers whose binary representation has 1's in all odd positions (counting from the MSB). */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert to zero‑based index */
    idx_t i = n - 1;

    /* Find the binary length L of the target term */
    idx_t len = 1;
    idx_t terms = (idx_t)1 << (len / 2);   /* 2^(len/2) terms of length len */
    while (i >= terms) {
        i    -= terms;
        ++len;
        terms = (idx_t)1 << (len / 2);
    }

    /* Build the number bit by bit */
    seq_t value = 0;
    idx_t  e = len / 2;        /* number of even positions */
    idx_t  evenIdx = 0;        /* index of current even position (from left) */
    idx_t  evenBits = i;       /* offset within this length group */

    for (idx_t pos = 1; pos <= len; ++pos) {
        seq_t bit;
        if (pos & 1) {           /* odd position -> always 1 */
            bit = 1;
        } else {                 /* even position -> bits come from evenBits in reverse order */
            bit = (evenBits >> (e - 1 - evenIdx)) & 1;
            ++evenIdx;
        }
        value = (value << 1) | bit;
    }
    return value;
}
