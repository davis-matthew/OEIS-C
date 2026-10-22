/*
 * A330393  A 2-regular sequence whose reciprocal is not 2-regular.
 * https://oeis.org/A330393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n) = n + 1, a(2n+1) = a(n) + 1, a(0) = 1
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == (idx_t)0) {
        return (seq_t)1;
    }
    if ((n & (idx_t)1) == (idx_t)0) {          /* even */
        return (seq_t)(n >> 1) + (seq_t)1;
    } else {                                   /* odd */
        return a(n >> 1) + (seq_t)1;
    }
}
