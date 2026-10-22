/*
 * A339085  Number of primes p with n - pi(n) < p <= n + pi(n), where pi(n) is
 *          the number of primes <= n.
 * https://oeis.org/A339085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi(n + pi(n)) - pi(max(0, n - pi(n)))
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    /* pi(n) */
    seq_t pi_n = compute_pi(n);

    /* upper bound n + pi(n) */
    /* cast to idx_t after addition to avoid overflow in seq_t */
    idx_t upper = (n > UINT64_MAX - (idx_t)pi_n) ? UINT64_MAX : (n + (idx_t)pi_n);
    seq_t pi_upper = compute_pi(upper);

    /* lower bound: max(0, n - pi(n)) */
    idx_t lower = (n > pi_n) ? (n - (idx_t)pi_n) : 0;
    seq_t pi_lower = compute_pi(lower);

    return pi_upper - pi_lower;
}
