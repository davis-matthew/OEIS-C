/*
 * A383242  a(n) = p(n)*p(n+1)*(p(n+1) - p(n)) + 1, where p(n) = prime(n).
 * https://oeis.org/A383242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n)*p(n+1)*(p(n+1)-p(n)) + 1
 * Range:     u8 n <= 3, u16 n <= 23, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1, n >= 1 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) /* offset protection, though n should be >=1 */
        return 0;

    /* Retrieve the n-th and (n+1)-th primes. */
    seq_t p1 = (seq_t)get_prime(n);
    seq_t p2 = (seq_t)get_prime(n + 1);

    /* Compute the formula. */
    return p1 * p2 * (p2 - p1) + 1;
}
