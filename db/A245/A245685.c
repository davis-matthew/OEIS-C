/*
 * A245685  Sigma(2p)/2, for odd primes p.
 * https://oeis.org/A245685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(prime(n+1)+1)/2 = sigma(2*prime(n+1))/2
 * Range:     u8 n <= 38, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

/* Computes the nth term of OEIS sequence A245685.  The index n is
 * 1‑based as required by the OEIS entry.  The function is safe for all
 * n >= 1 and works in O(1) time after the helper get_prime is
 * called, which internally uses a fast incremental prime generator. */

seq_t a(idx_t n)
{
    /* get the (n+1)th prime (1‑based) */
    uint32_t p = get_prime(n + 1);

    /* compute 3*(p+1)/2 with seq_t arithmetic to avoid overflow */
    seq_t prime = (seq_t)p;
    seq_t res = (seq_t)3 * (prime + 1) / 2;
    return res;
}
