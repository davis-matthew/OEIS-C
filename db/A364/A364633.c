/*
 * A364633  a(n) is the smallest nonnegative number k such that prime(n) + k
 *          is divisible by n + 1.
 * https://oeis.org/A364633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = (n+1)*ceil(prime(n)/(n+1)) - prime(n)
 * Range:     u8 n <= 54, u16 n <= 7647, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined for n ≥ 1.  */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Return 0 for n==0 (outside the defined range).  */
    if (n == 0)
        return (seq_t)0;

    /* Retrieve the n-th prime.  */
    seq_t p = (seq_t)get_prime(n);

    /* Compute n+1 in seq_t arithmetic.  */
    seq_t np1 = (seq_t)n + 1;

    /* Ceiling of p / (n+1).  */
    seq_t ceil = (p + np1 - 1) / np1;

    /* Result.  */
    return ceil * np1 - p;
}
