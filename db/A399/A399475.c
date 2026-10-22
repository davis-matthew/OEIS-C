/*
 * A399475  Position of the first appearance of n in A394762.
 * https://oeis.org/A399475
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-2) + floor(n*(n-3)/4) for n ≥ 7; a(2)=1, a(3)=3,
 *            a(4)=6, a(5)=11, a(6)=21
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Undefined behaviour for n < 2; return 0 for safety. */
    if (n < 2)
        return 0;

    /* Base cases for the first few terms. */
    if (n <= 6)
    {
        static const seq_t base[7] = {0, 0, 1, 3, 6, 11, 21};
        return base[n];
    }

    /* Compute 2^(n-2). */
    seq_t pow = pow2(n - 2);

    /* Compute floor(n*(n-3)/4). */
    seq_t extra = ((seq_t)n * ((seq_t)n - 3)) / 4;

    return pow + extra;
}
