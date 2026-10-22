/*
 * A366619  Group the natural numbers into blocks of size 2: [1,2], [3,4], ...
 *          and reverse the order of the numbers within each block. Then group
 *          into blocks of size 3 and reverse the order in each block.
 * https://oeis.org/A366619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*floor((n-1)/6) + base[(n-1) mod 6] where base =
 *            {4,1,2,5,6,3}. This implements the permutation described in
 *            A366619 and its period-6 property
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is 1-indexed; assume n >= 1 */
    static const seq_t base[6] = {4, 1, 2, 5, 6, 3};
    idx_t m = n - 1;                     /* 0-indexed */
    idx_t r = m % 6;                     /* remainder 0..5 */
    idx_t k = m / 6;                     /* quotient */
    seq_t result = (seq_t)base[r] + (seq_t)k * 6;
    return result;
}
