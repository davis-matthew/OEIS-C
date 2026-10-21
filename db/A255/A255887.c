/*
 * A255887  a(n) = 1 if the n-th prime is the sum of three squares, otherwise
 *          a(n) = 0.
 * https://oeis.org/A255887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by checking if the nth prime can be expressed as a sum of
   three squares: 1 if the nth prime is 2 or not ≡ 7 (mod 8), otherwise 0. */
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Get the nth prime (1‑indexed).  get_prime returns uint32_t. */
    seq_t p = (seq_t)get_prime(n);

    /* A prime can be represented as sum of three squares iff it is not
       of the form 4^k (8m + 7). For odd primes this is equivalent to
       p % 8 != 7. The prime 2 also qualifies. */
    if (p == (seq_t)2 || (p % (seq_t)8) != (seq_t)7)
        return (seq_t)1;
    else
        return (seq_t)0;
}
