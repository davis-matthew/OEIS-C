/*
 * A392143  a(n) = (n^(n - 1) - 1)^n.
 * https://oeis.org/A392143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^(n - 1) - 1)^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* cast n to seq_t for arbitrary-precision arithmetic */
    seq_t base = (seq_t)n;
    seq_t exp1 = (seq_t)(n - 1);

    /* compute n^(n-1) */
    seq_t pow1 = big_pow(base, exp1);

    /* compute (n^(n-1) - 1) */
    seq_t tmp = pow1 - (seq_t)1;

    /* compute ((n^(n-1) - 1)^n) */
    return big_pow(tmp, (seq_t)n);
}
