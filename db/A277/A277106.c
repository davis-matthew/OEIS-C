/*
 * A277106  a(n) = 8*3^n - 12.
 * https://oeis.org/A277106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*3^n - 12
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* Offset is 1 – behavior for n<1 is irrelevant */
    seq_t pow3 = 1;
    for (idx_t i = 1; i <= n; ++i)
        pow3 = pow3 * 3;          /* multiply by 3 each step (small int) */
    return (pow3 * 8) - 12;
}
