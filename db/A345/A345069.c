/*
 * A345069  Sums of two consecutive even-indexed primes.
 * https://oeis.org/A345069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(2*n) + prime(2*n+2)
 * Range:     u8 n <= 15, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

/* Return the nth term (n ≥ 1) of OEIS sequence A345069. */
seq_t a(idx_t n)
{
    if (n < 1u)
        return (seq_t)0;
    idx_t k1 = 2 * n;        /* index of prime(2n)   */
    idx_t k2 = k1 + 2;       /* index of prime(2n+2) */
    seq_t p1 = (seq_t)get_prime(k1);
    seq_t p2 = (seq_t)get_prime(k2);
    return p1 + p2;
}
