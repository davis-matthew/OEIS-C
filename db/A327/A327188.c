/*
 * A327188  For any n >= 0: consider the different ways to split the binary
 *          representation of n into two (possibly empty) parts, say with
 *          value x and y; a(n) is the greatest possible value of x AND y
 *          (where AND denotes the bitwise AND operator).
 * https://oeis.org/A327188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = maximum of (x & y) over all binary splits of n into left
 *            part x and right part y
 * Range:     u8 n <= 1355, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Compute the length in bits of a seq_t value (number of bits needed to represent it). */
static inline idx_t bitlen_seq(seq_t v) {
    idx_t len = 0;
    seq_t t = v;
    while (t != 0) {
        t >>= 1;
        len++;
    }
    return len;
}

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    idx_t len = bitlen_seq(sn);
    seq_t best = (seq_t)0;

    for (idx_t t = 0; t <= len; ++t) {
        seq_t x, y;
        if (t == 0) {
            x = (seq_t)0;
            y = sn;
        } else if (t == len) {
            x = sn;
            y = (seq_t)0;
        } else {
            idx_t shift = len - t;               /* number of bits in the right part */
            x = sn >> shift;                     /* left part */
            seq_t mask = ((seq_t)1 << shift) - (seq_t)1; /* mask for right part */
            y = sn & mask;
        }
        seq_t val = x & y;
        if (val > best)
            best = val;
    }

    return best;
}
