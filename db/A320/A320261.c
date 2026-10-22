/*
 * A320261  Write n in binary, then modify each run of 0's and each run of 1's
 *          by prepending a 1. a(n) is the decimal equivalent of the result.
 * https://oeis.org/A320261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   for a given n write its binary representation, then for every
 *            maximal run of equal bits prepend a 1 to that run. The resulting
 *            binary string is interpreted in base‑2
 * Range:     u8 n <= 17, u16 n <= 297, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* n >= 1 */
    seq_t out = 0;
    int bits = 0;
    idx_t tmp = n;
    /* count number of bits needed to represent n */
    while (tmp) {
        bits++;
        tmp >>= 1;
    }

    int prev = -1;          /* no previous bit yet */
    for (int i = bits - 1; i >= 0; --i) {
        int cur = (n >> i) & 1;
        if (cur != prev) {
            /* new run: prepend a 1 */
            out = (out << 1) | 1;
        }
        /* copy the current bit */
        out = (out << 1) | cur;
        prev = cur;
    }
    return out;
}
