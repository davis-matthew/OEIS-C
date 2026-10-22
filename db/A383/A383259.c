/*
 * A383259  a(n) is the excess of even composites over odd composites in the
 *          first n positive integers.
 * https://oeis.org/A383259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = pi(n) - (n mod 2) - 1 for n > 1; a(1) = 0
 * Range:     u8 n <= 1621, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t pi_n = compute_pi(n);          /* number of primes <= n */
    seq_t n_mod2 = (seq_t)(n & 1);        /* n mod 2 */

    return pi_n - n_mod2 - 1;
}
