/*
 * A392034  a(n) = prime(n)*(prime(n+1) - prime(n))/2.
 * https://oeis.org/A392034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = prime(n)*(prime(n+1) - prime(n))/2
 * Range:     u8 n <= 23, u16 n <= 649, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Fetch consecutive primes */
    seq_t p_n  = (seq_t)get_prime(n);
    seq_t p_n1 = (seq_t)get_prime(n + 1);

    /* Compute the value: p_n * (p_n1 - p_n) / 2 */
    return (p_n * (p_n1 - p_n)) / 2;
}
