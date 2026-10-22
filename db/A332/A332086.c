/*
 * A332086  a(n) = pi(prime(n) + n) - n, where pi is the prime counting
 *          function.
 * https://oeis.org/A332086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi(prime(n) + n) - n
 * Range:     u8 n <= 2609, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/compute_pi.h"   /* seq_t compute_pi(idx_t n); */
#include "../utils/get_prime.h"    /* uint32_t get_prime(idx_t k); */

seq_t a(idx_t n)
{
    /* prime(n) is 1‑based; get_prime returns the nth prime as uint32_t */
    uint32_t p = get_prime(n);
    /* compute the argument of pi: prime(n) + n */
    idx_t  sum = (idx_t)p + n;
    /* pi(sum) gives the number of primes ≤ sum */
    seq_t pi_sum = compute_pi(sum);
    /* subtract n to obtain a(n) */
    seq_t result = pi_sum - (seq_t)n;
    return result;
}
