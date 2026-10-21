/*
 * A266284  Decimal representation of the n-th iteration of the "Rule 13"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*2^n-1)/3 for even n; a(n) = 2*4^n - (8*2^n-1)/3 - 1
 *            for odd n
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t pow2n = pow2(n);
    if ((n & 1) == 0) {         /* even n */
        seq_t t = ((seq_t)4) * pow2n;
        t -= 1;
        return t / 3;
    } else {                    /* odd n */
        seq_t pow2n2 = pow2n * pow2n;   /* 2^(2n) = (2^n)^2 */
        seq_t t1 = ((seq_t)2) * pow2n2; /* 2*4^n */
        seq_t t2 = ((seq_t)8) * pow2n; /* 8*2^n */
        t2 -= 1;
        t2 /= 3;
        return t1 - t2 - 1;
    }
}
