/*
 * A262459  Number of n-valued logics.
 * https://oeis.org/A262459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)^(n^2+1) * n^(n-2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   big_pow, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    seq_t base1 = (seq_t)(n - 1);   /* n-1 */
    seq_t base2 = (seq_t)n;         /* n */

    /* exponent for (n-1) : n^2 + 1 */
    __uint128_t nn = (__uint128_t)n * n;
    seq_t exp1 = (seq_t)nn + (seq_t)1;

    /* exponent for n : n-2 */
    seq_t exp2 = (seq_t)n - (seq_t)2;

    seq_t pow1 = big_pow(base1, exp1);
    seq_t pow2 = big_pow(base2, exp2);

    return seq_mul(pow1, pow2);
}
