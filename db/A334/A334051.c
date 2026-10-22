/*
 * A334051  The difference between the number of prime numbers in the ranges
 *          (1, p_n] and (p_n, 2*p_n], where p_n is the n-th prime.
 * https://oeis.org/A334051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n - pi(2*p_n) where p_n is the n-th prime and pi(x) is
 *            the number of primes ≤ x
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   get_prime, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/get_prime.h"   /* extern uint32_t get_prime(idx_t k); */
#include "../utils/prime_pi.h"   /* extern seq_t prime_pi(idx_t limit); */

/* Computes the A334051 sequence term for index n (1‑based). */
seq_t a(idx_t n)
{
    /* Defend against the (undefined) zero index. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Get the n-th prime (p_n). */
    uint32_t pn = get_prime(n);

    /* Compute pi(2*p_n). */
    idx_t limit = (idx_t)pn * (idx_t)2U;
    seq_t pi_val = prime_pi(limit);

    /* a(n) = 2*n - pi(2*p_n) */
    return (seq_t)n * (seq_t)2U - pi_val;
}
