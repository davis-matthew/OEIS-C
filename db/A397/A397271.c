/*
 * A397271  The value on the left of the rightmost pair of zero bits of n in
 *          binary.
 * https://oeis.org/A397271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = value left of the rightmost pair of adjacent zero bits in
 *            binary representation of n
 * Range:     u8 n <= 1023, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    int pos = -1;
    /* Scan for the lowest position where two consecutive bits are 0. */
    for (int i = 0; i < 63; ++i) {
        if ( ((n >> i) & 3ULL) == 0ULL ) {
            pos = i;
            break;
        }
    }
    if (pos < 0) return 0;
    if (pos + 2 >= 64) return 0;        /* shifting by 64 is undefined */
    return (seq_t)(n >> (pos + 2));
}
