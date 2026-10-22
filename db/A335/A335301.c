/*
 * A335301  a(n) = prime(n+1) mod (10^k) where k is the least positive integer
 *          such that floor(prime(n)/(10^k)) = floor(prime(n+1)/(10^k)) and
 *          prime(n) denotes the n-th prime number.
 * https://oeis.org/A335301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = prime(n+1) mod (10^k) where k is the least positive
 *            integer such that floor(prime(n)/10^k) = floor(prime(n+1)/10^k)
 * Range:     u8 n <= 53, u16 n <= 6652, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

/* Compute the n‑th term of OEIS sequence A335301 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* undefined for n < 1, return 0 */

    /* Retrieve consecutive primes. */
    seq_t p = (seq_t)get_prime(n);
    seq_t q = (seq_t)get_prime(n + 1);

    /* Find the lowest power of 10 for which p and q share the same prefix. */
    seq_t pow10 = (seq_t)10;
    while (p / pow10 != q / pow10)
        pow10 *= (seq_t)10;

    return q % pow10;                     /* the required suffix */
}
