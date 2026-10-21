/*
 * A274269  a(n) = (5*n - 1)^(n-1).
 * https://oeis.org/A274269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n - 1)^(n-1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A274269: a(n) = (5*n - 1)^(n-1) */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1 (offset 1).  For n == 0 the
       behaviour is irrelevant, but we return 0 to avoid undefined
       behaviour. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Compute base = 5*n - 1 as a seq_t. */
    seq_t base = (seq_t)(5 * n) - 1;

    /* Exponent = n - 1 as a seq_t. */
    seq_t exp = (seq_t)(n - 1);

    /* Use the provided big_pow utility to perform exponentiation. */
    return big_pow(base, exp);
}
