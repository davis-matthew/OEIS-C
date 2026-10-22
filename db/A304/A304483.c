/*
 * A304483  a(n) = pi(n)*pi(2n), where pi is A000720: the prime counting
 *          function.
 * https://oeis.org/A304483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = pi(n) * pi(2n)
 * Range:     u8 n <= 39, u16 n <= 1155, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    seq_t pi_n  = compute_pi(n);
    seq_t pi_2n = compute_pi(n * ((idx_t)2));
    return pi_n * pi_2n;
}
