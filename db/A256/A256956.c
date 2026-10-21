/*
 * A256956  a(n) = pi(n) * pi(n+1), where pi(n) is the number of primes <= n.
 * https://oeis.org/A256956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = pi(n) * pi(n+1)
 * Range:     u8 n <= 52, u16 n <= 1618, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    seq_t pi_n   = compute_pi(n);
    seq_t pi_np1 = compute_pi(n + 1);
    return pi_n * pi_np1;
}
