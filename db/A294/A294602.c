/*
 * A294602  a(n) = pi(n-1) - pi(floor(n/2)), where pi is A000720.
 * https://oeis.org/A294602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = pi(n-1) - pi(floor(n/2))
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    idx_t n_minus_1 = n - 1;
    idx_t n_half    = n / 2;
    seq_t pi_n_minus_1 = prime_pi(n_minus_1);
    seq_t pi_n_half    = prime_pi(n_half);
    return pi_n_minus_1 - pi_n_half;
}
