/*
 * A321017  a(n) = floor(pi(n)/2).
 * https://oeis.org/A321017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(pi(n)/2)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    /* compute the prime counting function pi(n) */
    seq_t prime_count = compute_pi(n);
    /* return its half, truncated (floor) */
    return prime_count / 2;
}
