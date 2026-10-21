/*
 * A266380  Decimal representation of the n-th iteration of the "Rule 21"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=3; for n>1: a(n)=0 if n is even,
 *            a(n)=2^(4*floor(n/2)+3)-1 if n is odd
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if ((n & 1) == 0) return (seq_t)0;          /* even n ≥ 2 */

    /* odd n: n=2k+1, k = floor(n/2) */
    idx_t k = n >> 1;
    seq_t val = pow2(k * 4 + 3);                /* 2^(4k+3) */
    return val - (seq_t)1;
}
