/*
 * A325413  Largest sum of the omega-sequence of an integer partition of n.
 * https://oeis.org/A325413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1, a(2)=3, a(3)=5, a(4)=8, a(5)=9, a(6)=10,
 *            a(n)=n+5 for n>=7
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 3;
    if (n == 3) return 5;
    if (n == 4) return 8;
    if (n == 5) return 9;
    if (n == 6) return 10;
    return ((seq_t)n) + 5;
}
