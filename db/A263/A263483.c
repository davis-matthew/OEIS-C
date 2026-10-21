/*
 * A263483  a(n) = prime(n)+(prime(n) modulo 6).
 * https://oeis.org/A263483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) + (prime(n) mod 6)
 * Range:     u8 n <= 53, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term by fetching the n‑th prime and adding its remainder modulo 6 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Retrieve the n-th prime (n ≥ 1) */
    seq_t p = (seq_t)get_prime(n);   /* get_prime returns uint32_t */
    seq_t rem = p % (seq_t)6;
    return p + rem;
}
