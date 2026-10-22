/*
 * A318164  a(n) = A000225(n)^A000217(n-1), n > 0.
 * https://oeis.org/A318164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^n - 1)^(n*(n - 1)/2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   big_pow, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* base = 2^n - 1 */
    seq_t base = pow2(n);
    base -= 1;

    /* exponent = n*(n-1)/2 */
    seq_t exp = (seq_t)n * ((seq_t)n - 1) / 2;

    return big_pow(base, exp);
}
