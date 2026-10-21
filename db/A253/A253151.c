/*
 * A253151  Number of (n+1)X(n+1) 0..1 arrays with every 2X2 subblock diagonal
 *          minus antidiagonal sum nondecreasing horizontally and vertically.
 * https://oeis.org/A253151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 18*2^(n-1) + 20 for n > 3; a(1) = 16, a(2) = 72, a(3) =
 *            94
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)16;
    if (n == 2) return (seq_t)72;
    if (n == 3) return (seq_t)94;
    /* n >= 4 */
    seq_t t = pow2(n - 1);          /* t = 2^(n-1) */
    return t * (seq_t)18 + (seq_t)20;
}
