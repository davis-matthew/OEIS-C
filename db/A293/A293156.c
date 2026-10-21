/*
 * A293156  Number of linear chord diagrams with n+2 chords such that every
 *          chord has length at least n.
 * https://oeis.org/A293156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 292*3^(n-4) for n>3; a(1)=15, a(2)=36, a(3)=99
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 1) return 15;
    if (n == 2) return 36;
    if (n == 3) return 99;
    /* n ≥ 4 */
    seq_t exp = (seq_t)(n - 4);
    seq_t pow3 = big_pow((seq_t)3, exp);
    return (seq_t)292 * pow3;
}
