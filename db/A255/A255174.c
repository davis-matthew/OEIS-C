/*
 * A255174  a(n) = prime(3*n) - prime(2*n).
 * https://oeis.org/A255174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(3*n) - prime(2*n)
 * Range:     u8 n <= 40, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

/* Return the n-th term of the OEIS sequence A255174 */
seq_t a(idx_t n)
{
    /* OEIS offset is 1; value for n==0 is undefined, return 0 for safety */
    if (n == 0)
        return 0;

    /* Compute the (3*n)-th and (2*n)-th primes and return their difference */
    seq_t p3 = (seq_t)get_prime(3 * n);
    seq_t p2 = (seq_t)get_prime(2 * n);
    return p3 - p2;
}
