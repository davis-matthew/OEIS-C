/*
 * A253437  Number of (3+1) X (n+1) 0..1 arrays with every 2 X 2 subblock
 *          diagonal minus antidiagonal sum nondecreasing horizontally,
 *          vertically and ne-to-sw antidiagonally.
 * https://oeis.org/A253437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=69, a(2)=73, a(3)=85; a(n) = 9*2^(n-1) + 48 for n > 3 (OEIS
 *            notation)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* Handle the offset and initial values */
    if (n == 0) return 0;            /* arbitrary */
    if (n == 1) return 69;
    if (n == 2) return 73;
    if (n == 3) return 85;

    /* For n > 3 use the explicit formula: 9*2^(n-1) + 48 */
    seq_t pow2_val = pow2(n - 1);    /* 2^(n-1) */
    return 9 * pow2_val + 48;
}
