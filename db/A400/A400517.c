/*
 * A400517  a(1) = 1; for n > 1, a(n) is the least integer > a(n-1) such that
 *          the binary expansion of a(n-1)^2 is a subsequence of the binary
 *          expansion of a(n)^2.
 * https://oeis.org/A400517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, new
 * Formula:   a(1)=1; a(2)=2; a(3)=3; a(4)=5; a(5)=7; a(6)=9; a(7)=13;
 *            a(8)=19; a(9)=27; a(n)=2^{n-5}+5 for n≥10
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 36, u64 n <= 68, u128 n <= 100,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    static const seq_t init[10] = {0,1,2,3,5,7,9,13,19,27};
    if (n < 10) return init[n];
    return pow2(n-5) + 5;
}
