/*
 * A390577  Remainder when (prime(n) * prime(n+1)) is divided by n.
 * https://oeis.org/A390577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (prime(n) * prime(n+1)) mod n
 * Range:     u8 n <= 53, u16 n <= 8333, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

/* Returns the n‑th term of sequence A390577.
   For n >= 1 it is (prime(n) * prime(n+1)) modulo n.
   Uses get_prime(idx_t) from the utilities to obtain the primes. */
seq_t a(idx_t n)
{
    if (n == 0)           /* offset starts at 1; safe guard */
        return 0;

    seq_t mod = (seq_t)n;                       /* cast to seq_t for uniform arithmetic */
    seq_t p1   = get_prime(n);                  /* prime(n)  */
    seq_t p2   = get_prime(n + 1);              /* prime(n+1) */
    seq_t r1 = p1 % mod;                        /* reduce modulo n */
    seq_t r2 = p2 % mod;                        /* reduce modulo n */
    return (r1 * r2) % mod;                     /* product modulo n */
}
