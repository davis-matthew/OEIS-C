/*
 * A260006  a(n) = f(1,n,n), where f is the Sudan function defined in A260002.
 * https://oeis.org/A260006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = (2^n - 1) * (n + 2)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute 2^n using the provided big_pow helper. */
    seq_t pow2 = big_pow((seq_t)2, (seq_t)n);

    /* (n + 2) as a seq_t value. */
    seq_t n_plus_2 = (seq_t)n + 2;

    /* Return (2^n - 1) * (n + 2). */
    return (pow2 - 1) * n_plus_2;
}
