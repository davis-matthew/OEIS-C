/*
 * A267681  Decimal representation of the n-th iteration of the "Rule 201"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*4^n - (n%2*2 + 5)*2^(n-1) - 1, with [n] = 1 iff n>0
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    if (n == 0)            /* base case */
        return (seq_t)1;

    /* powers of 2 and 4 */
    seq_t pow4_n   = pow4(n);           /* 4^n   */
    seq_t pow2_n1  = pow2(n - 1);       /* 2^(n-1) */

    /* 2*4^n */
    seq_t part1 = ((seq_t)2) * pow4_n;

    /* (n%2)*2 + 5  (since n>0) */
    seq_t mult = ((n & (idx_t)1) ? (seq_t)2 : (seq_t)0) + (seq_t)5;

    /* ( (n%2)*2 + 5 ) * 2^(n-1) */
    seq_t part2 = mult * pow2_n1;

    /* final value */
    return part1 - part2 - (seq_t)1;
}
