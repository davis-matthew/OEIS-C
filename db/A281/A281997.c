/*
 * A281997  a(n) = (n-1)^n * n^n.
 * https://oeis.org/A281997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n-1)^n * n^n
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset starts at 1, but guard just in case */
    seq_t base1 = (seq_t)(n - 1);        /* (n-1) */
    seq_t base2 = (seq_t)n;              /* n       */
    seq_t exp   = (seq_t)n;              /* exponent n */

    seq_t pow1 = big_pow(base1, exp);    /* (n-1)^n */
    seq_t pow2 = big_pow(base2, exp);    /* n^n   */

    return pow1 * pow2;                  /* (n-1)^n * n^n */
}
