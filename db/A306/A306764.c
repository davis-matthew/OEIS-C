/*
 * A306764  a(n) is a sequence of period 12: repeat [1, 1, 6, 2, 1, 3, 2, 2,
 *          3, 1, 2, 6].
 * https://oeis.org/A306764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [1, 1, 6, 2, 1, 3, 2, 2, 3, 1, 2, 6][n mod 12]
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t pat[12] = {1, 1, 6, 2, 1, 3, 2, 2, 3, 1, 2, 6};
    return pat[(size_t)(n % 12U)];
}
