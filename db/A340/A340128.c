/*
 * A340128  a(n) = (n*prime(n)) mod prime(n+1).
 * https://oeis.org/A340128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = (n * prime(n)) mod prime(n+1)
 * Range:     u8 n <= 60, u16 n <= 7392, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Fetch the n-th and (n+1)-th primes (1‑indexed). */
    uint64_t p_n   = get_prime(n);
    uint64_t p_np1 = get_prime(n + 1);

    /* Compute n * p_n modulo p_np1 using a wider integer type to avoid overflow. */
    __uint128_t prod = ( __uint128_t )n * ( __uint128_t )p_n;
    return ( seq_t )( prod % ( __uint128_t )p_np1 );
}
