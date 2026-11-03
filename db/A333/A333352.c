/*
 * A333352  a(n) is the product of indices of the smallest and greatest prime
 *          factors of n.
 * https://oeis.org/A333352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi(spf(n)) * pi(gpf(n)) where spf and gpf are the
 *            smallest and greatest prime factors of n, and pi(x) is the
 *            number of primes <= x
 * Range:     u8 n <= 52, u16 n <= 1618, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 9604 (2026-11-01)
 * Helpers:   largest_prime_factor, prime_pi, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/largest_prime_factor.h"
#include "../utils/prime_pi.h"
#include "../utils/smallest_prime_factor.h"

seq_t a(idx_t n)
{
    /* By definition a(1) = 1. */
    if (n <= 1)
        return (seq_t)1;

    /* Smallest and greatest prime factors of n. */
    uint64_t spf = smallest_prime_factor(n);
    uint64_t gpf = largest_prime_factor(n);

    /* pi for each factor (returns type seq_t). */
    seq_t pi_spf = prime_pi(spf);
    seq_t pi_gpf = prime_pi(gpf);

    /* Final result.  For built‑in integer seq_t types the operator
       * is fine; for arbitrary‑precision seq_t it is implemented as
       * operator*.  No additional cast is necessary. */
    return pi_spf * pi_gpf;
}
