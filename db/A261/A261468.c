/*
 * A261468  a(n) = prime(n+2) mod prime(n).
 * https://oeis.org/A261468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n+2) mod prime(n) (offset 1)
 * Range:     u8 n <= 52, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Our function supports all n >= 1. For n < 1 we return 0. */
    if (n < 1)
        return 0;

    /* Retrieve the required primes.  get_prime(k) returns the k‑th prime
       (1-indexed) as a native integer (uint32_t).  Convert them to seq_t
       before performing arithmetic, which keeps the code valid for
       arbitrary-precision builds. */
    seq_t p  = (seq_t)get_prime(n);
    seq_t p2 = (seq_t)get_prime(n + 2);

    return p2 % p;   /* mod operation works for all supported seq_t types */
}
