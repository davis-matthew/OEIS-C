/*
 * A307989  a(n) = n - pi(2*n) + pi(n-1), where pi is the prime counting
 *          function.
 * https://oeis.org/A307989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - pi(2*n) + pi(n-1)
 * Range:     u8 n <= 304, u16 n <= 8902, u32 n <= 8902, u64 n <= 8902,
 *            u128 n <= 8902, big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not part of the OEIS offset */
    seq_t pi2 = compute_pi(n * 2);
    seq_t pin1 = (n > 1) ? compute_pi(n - 1) : 0;
    return (seq_t)n - pi2 + pin1;
}
