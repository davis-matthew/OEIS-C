/*
 * A352749  a(n) = pi(n) * (pi(2n-1) - pi(n-1)).
 * https://oeis.org/A352749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = pi(n) * (pi(2n-1) - pi(n-1))
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/compute_pi.h"

/* Computes the number of ordered pairs (p,q) of primes with
   p <= n <= q < 2n.  The function works for any n >= 1. */
seq_t a(idx_t n)
{
    /* Guard against the offset: for n==0 return 0 (should not occur). */
    if (n == 0) return 0;

    /* Compute the auxiliary indices. */
    idx_t n_minus_1      = n - 1;
    idx_t two_n_minus_1  = (idx_t)((n << 1) - 1);

    /* Obtain the prime counts. */
    seq_t pi_n            = compute_pi(n);
    seq_t pi_n_minus1     = compute_pi(n_minus_1);
    seq_t pi_two_n_minus1 = compute_pi(two_n_minus_1);

    /* Return the required product. */
    return pi_n * (pi_two_n_minus1 - pi_n_minus1);
}
