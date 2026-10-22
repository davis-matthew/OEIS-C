/*
 * A332124  a(n) = 2*(10^(2n+1)-1)/9 + 2*10^n.
 * https://oeis.org/A332124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 2*(10^(2n+1)-1)/9 + 2*10^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Compute 2*(10^(2n+1)-1)/9 */
    idx_t exp1   = 2*n + 1;
    seq_t pow1   = pow10(exp1);            /* 10^(2n+1)  */
    seq_t part1  = (pow1 - (seq_t)1) / (seq_t)9;
    part1       *= (seq_t)2;                /* 2 * ((10^(2n+1)-1)/9) */

    /* Compute 2*10^n */
    seq_t pow2   = pow10(n);                /* 10^n        */
    seq_t part2  = (seq_t)2 * pow2;         /* 2*10^n      */

    return part1 + part2;
}
