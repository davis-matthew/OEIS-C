/*
 * A271511  a(n) = (p+1)*(p+2)/2 where p is the n-th prime.
 * https://oeis.org/A271511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (p+1)*(p+2)/2 where p is the n-th prime
 * Range:     u8 n <= 6, u16 n <= 57, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Retrieve the n-th prime.  The utility returns a uint32_t, which
       is safely cast to seq_t. */
    uint32_t p32 = get_prime(n);
    seq_t p = (seq_t)p32;

    /* Compute (p+1)*(p+2)/2 using seq_t arithmetic only. */
    seq_t t = (p + (seq_t)1) * (p + (seq_t)2);
    return t / (seq_t)2;
}
