/*
 * A263800  Numbers n such that there is some field with u-invariant n.
 * https://oeis.org/A263800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  hard, nonn, more
 * Formula:   a(1)=1; a(2)=2; a(3)=4; a(4)=6; a(5)=8; a(6)=9; a(7)=10;
 *            a(n)=2*(n-2) for n>=8
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)4;
    if (n == 4) return (seq_t)6;
    if (n == 5) return (seq_t)8;
    if (n == 6) return (seq_t)9;
    if (n == 7) return (seq_t)10;
    return (seq_t)(n - 2) * (seq_t)2;
}
