/*
 * A246514  Number of composite numbers between prime(n) and 2*prime(n)
 *          exclusive.
 * https://oeis.org/A246514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)-1 - (π(2*prime(n)-1) - π(prime(n)))
 * Range:     u8 n <= 54, u16 n <= 6990, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    /* Valid for n >= 1; return 0 for n < 1 (undefined offset) */
    if (n < 1) return (seq_t)0;

    /* 1. Obtain the n-th prime */
    seq_t p = (seq_t)get_prime(n);

    /* 2. Count primes in the interval (p, 2p)  
       π(2p-1) gives primes ≤ 2p-1, π(p) gives primes ≤ p */
    idx_t two_p_minus1 = (idx_t)(2 * (idx_t)p - 1);
    seq_t pi_2p_minus1 = compute_pi(two_p_minus1);
    seq_t pi_p = compute_pi((idx_t)p);

    /* 3. Number of primes strictly between p and 2p */
    seq_t primes_in_interval = pi_2p_minus1 - pi_p;

    /* 4. Total integers in (p, 2p) is p-1; subtract primes to get composites */
    seq_t result = p - (seq_t)1 - primes_in_interval;
    return result;
}
