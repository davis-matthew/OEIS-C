/*
 * A326011  a(n) = (n+1) * (2^n + 1)^n.
 * https://oeis.org/A326011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1) * (2^n + 1)^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   big_pow, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* compute 2^n */
    seq_t pow2_n = pow2(n);

    /* base = 2^n + 1 */
    seq_t base = pow2_n;
    base += 1;

    /* power = base^n */
    seq_t power = big_pow(base, (seq_t)n);

    /* multiply by (n+1) */
    seq_t n1 = (seq_t)(n + 1);
    seq_t result = n1 * power;

    return result;
}
