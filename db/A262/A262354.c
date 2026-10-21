/*
 * A262354  a(n) is the number of 2 X 2 matrices over Z_p with determinant in
 *          {1,-1} where p = prime(n).
 * https://oeis.org/A262354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 6; for n > 1, a(n) = 2*p*(p^2-1) where p = prime(n)
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)6;
    }

    /* p is the n-th prime */
    seq_t p = (seq_t)get_prime(n);

    /* Compute 2 * p * (p^2 - 1) */
    seq_t p2 = p * p;
    seq_t result = (seq_t)2 * p * (p2 - (seq_t)1);
    return result;
}
