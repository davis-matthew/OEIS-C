/*
 * A253428  Number of (n+1)X(n+1) 0..1 arrays with every 2X2 subblock diagonal
 *          minus antidiagonal sum nondecreasing horizontally, vertically and
 *          ne-to-sw antidiagonally.
 * https://oeis.org/A253428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12 + 9*2^n for n >= 4 a(1) = 16, a(2) = 58, a(3) = 85
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == (idx_t)1) return (seq_t)16;
    if (n == (idx_t)2) return (seq_t)58;
    if (n == (idx_t)3) return (seq_t)85;
    /* n >= 4: use the closed form */
    seq_t pow = pow2(n);          /* 2^n */
    return (seq_t)12 + (seq_t)9 * pow;
}
