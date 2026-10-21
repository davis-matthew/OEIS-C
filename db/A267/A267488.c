/*
 * A267488  Smallest b > 1 such that there exists an odd prime p with p < b
 *          such that b^(p-1) == 1 (mod p^n).
 * https://oeis.org/A267488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 81*3^(n-4)-1 for n ≥ 4; a(1)=4, a(2)=7, a(3)=18
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* handle the first three terms explicitly */
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)18;

    /* for n ≥ 4: a(n) = 81 * 3^(n-4) - 1 */
    seq_t exp   = (seq_t)(n - 4);   /* exponent as seq_t */
    seq_t val   = big_pow((seq_t)3, exp);   /* 3^(n-4) */
    val         = val * (seq_t)81;           /* multiply by 81 */
    val         = val - (seq_t)1;            /* subtract 1 */
    return val;
}
