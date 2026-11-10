/*
 * A244527  Bisection of A030190 or of A030302.
 * https://oeis.org/A244527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A030302(2n-1), the (2n-1)-th bit of the binary
 *            Champernowne word 1,10,11,100,
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t rem = 2 * n - 2; /* 0-based position of bit 2n-1 */
    idx_t len = 1;
    while (1) {
        idx_t bits = (((idx_t)1) << (len - 1)) * len;
        if (rem < bits) break;
        rem -= bits;
        len++;
    }
    idx_t index = rem / len;
    idx_t offset = rem % len;
    idx_t number = (((idx_t)1) << (len - 1)) + index;
    idx_t shift = len - 1 - offset;
    return (seq_t)((number >> shift) & 1);
}
