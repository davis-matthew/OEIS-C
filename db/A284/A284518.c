/*
 * A284518  Periodic with period [1, 10, 9, 16, 1, 18, 1, 16, 9, 10, 1, 24] of
 *          length 12.
 * https://oeis.org/A284518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24 - 15*n + 30*floor(n/2) + 8*floor(n/3) -
 *            8*floor((n+2)/3) - 6*floor((n+2)/4) + 6*floor((n+1)/4)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute using 64‑bit signed arithmetic to avoid underflow. */
    int64_t N = (int64_t)n;
    int64_t res = 24
        - 15 * N
        + 30 * (N / 2)
        + 8  * (N / 3)
        - 8  * ((N + 2) / 3)
        - 6  * ((N + 2) / 4)
        + 6  * ((N + 1) / 4);
    return (seq_t)res;
}
