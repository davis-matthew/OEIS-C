/*
 * A338215  a(n) = A095117(A062298(n)).
 * https://oeis.org/A338215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - pi(n) + pi(n - pi(n))
 * Range:     u8 n <= 265, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    /* pi(n) */
    seq_t pi_n = compute_pi(n);
    /* n - pi(n) */
    idx_t n_minus_pi = n - (idx_t)pi_n;
    /* pi(n - pi(n)) */
    seq_t pi2 = compute_pi(n_minus_pi);
    /* return n - pi(n) + pi(n - pi(n)) */
    return (seq_t)n - pi_n + pi2;
}
