/*
 * A289495  Number of primes in the interval [4n, 5n].
 * https://oeis.org/A289495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = π(5*n) – π(4*n-1)
 * Range:     u8 n <= 2423, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    /* Sequence is defined for n ≥ 1; behaviour for n < 1 is irrelevant. */
    idx_t lower = 4 * n;   /* first end of the interval */
    idx_t upper = 5 * n;   /* last  end of the interval */

    /* Count primes ≤ 5n and ≤ (4n-1) and subtract. */
    seq_t pi_upper   = prime_pi(upper);
    seq_t pi_lower_x = prime_pi(lower - 1);

    return pi_upper - pi_lower_x;
}
