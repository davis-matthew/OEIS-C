/*
 * A321018  a(n) = round(pi(n)/2).
 * https://oeis.org/A321018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(pi(n)/2)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    seq_t pc = compute_pi(n);          /* pi(n) */
    return (pc + 1) / 2;               /* round(pi(n)/2) = floor((pi(n)+1)/2) */
}
