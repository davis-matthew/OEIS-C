/*
 * A375105  a(n) = (2*n)^(n-2)*(2*n)!/n!.
 * https://oeis.org/A375105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n)^(n-2)*(2*n)!/n!
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* For the offset n = 1 the exponent becomes –1, so we handle it
       explicitly.  The result equals 1. */
    if (n <= 1)
        return (seq_t)1;

    /* Compute (2*n)^(n-2) using the provided big_pow helper. */
    seq_t base = (seq_t)(2 * n);
    seq_t pow_term = big_pow(base, (seq_t)(n - 2));

    /* Compute the factorial ratio (2*n)! / n! =
       product_{k=n+1}^{2*n} k  */
    seq_t prod = (seq_t)1;
    for (idx_t k = n + 1; k <= 2 * n; ++k)
        prod *= (seq_t)k;

    return pow_term * prod;
}
