/*
 * A251720  a(n) = (p_n)^2 * p_{n+1}, where p_n is the n-th prime, A000040(n).
 * https://oeis.org/A251720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000040(n) * A000040(n) * A000040(n+1)
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 257, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* OEIS offset is 1: a(1) corresponds to the first pair of primes. */
    if (n == 0) return 0;   /* defined behaviour for invalid indices */

    /* Retrieve the n-th and (n+1)-th primes. */
    uint32_t p  = get_prime(n);
    uint32_t pn = get_prime(n + 1);

    /* Compute (p)^2 * pn in seq_t type. */
    seq_t result = (seq_t)p;
    result *= result;
    result *= (seq_t)pn;

    return result;
}
