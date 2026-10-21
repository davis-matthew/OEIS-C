/*
 * A290564  Number of primes between n^2 and 2*n^2.
 * https://oeis.org/A290564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = pi(2*n^2) - pi(n^2) (number of primes between n^2 and
 *            2*n^2)
 * Range:     u8 n <= 45, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    /* n is an unsigned 64‑bit integer.  All intermediate values fit in idx_t. */
    idx_t n2      = n * n;           /* n^2                  */
    idx_t two_n2  = n2 * (idx_t)2;   /* 2*n^2                */

    seq_t pi_hi = compute_pi(two_n2); /* pi(2*n^2) */
    seq_t pi_lo = compute_pi(n2);     /* pi(n^2)   */

    return pi_hi - pi_lo;
}
