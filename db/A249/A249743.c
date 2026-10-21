/*
 * A249743  Main diagonal of square arrays A114881 and A249741.
 * https://oeis.org/A249743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p_n * p_{2*(n-1)} - 1, where p_k is the k-th prime. a(1)
 *            is defined as 1
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* a(1) = 1 */
    if (n == 1)
        return (seq_t)1;

    /* Compute indexes for the required primes */
    idx_t m = n - 1;   /* (n-1) */
    m = m + m;         /* 2 * (n-1) */

    /* Retrieve the primes and compute the value */
    seq_t p1 = (seq_t)get_prime(n);
    seq_t p2 = (seq_t)get_prime(m);
    return p1 * p2 - (seq_t)1;
}
