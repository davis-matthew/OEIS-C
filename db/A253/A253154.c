/*
 * A253154  Number of (n+1) X (3+1) 0..1 arrays with every 2 X 2 subblock
 *          diagonal minus antidiagonal sum nondecreasing horizontally and
 *          vertically.
 * https://oeis.org/A253154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*2^(n-1) + 57 for n > 3 with a(1)=69, a(2)=88, a(3)=94
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)69;
    if (n == 2)
        return (seq_t)88;
    if (n == 3)
        return (seq_t)94;

    seq_t pow2 = (seq_t)1;
    /* Compute 2^(n-1) */
    for (idx_t i = 1; i < n; ++i)
        pow2 = pow2 * (seq_t)2;

    seq_t result = pow2 * (seq_t)9 + (seq_t)57;
    return result;
}
