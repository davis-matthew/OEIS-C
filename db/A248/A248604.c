/*
 * A248604  Numbers a(n) which are the minimum number of moves needed in a
 *          variation of the tower of Hanoi with 4 towers and n disks.
 * https://oeis.org/A248604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5 + 2^(n-2) for n > 3, with a(1)=1, a(2)=3, a(3)=5
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 1)  return (seq_t)1;
    if (n == 2)  return (seq_t)3;
    if (n == 3)  return (seq_t)5;

    /* n ≥ 4: a(n) = 5 + 2^(n-2) */
    seq_t pow2 = pow_uint(2ULL, n - 2);
    return pow2 + (seq_t)5;
}
